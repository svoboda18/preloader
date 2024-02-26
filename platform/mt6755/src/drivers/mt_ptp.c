/*=============================================================
 * Include files
 *=============================================================*/
#include "platform.h"
#include "typedefs.h"
#include "sec_devinfo.h"
#include "mt_ptp.h"
#include "pll.h"
//#include "platform.h"

extern U32 upmu_get_reg_value(kal_uint32 reg);


/*=============================================================
 * Macro definition
 *=============================================================*/

#define __stringify_1(x...)	#x
#define __stringify(x...)	__stringify_1(x)

#define ARRAY_SIZE(x) sizeof(x)/sizeof(x[0])

#define CONFIG_PTP_SHOWLOG	1
#define EN_ISR_LOG		0
#define NR_FREQ			8

/*
 * 100 us, This is the PTP Detector sampling time as represented in
 * cycles of bclk_ck during INIT. 52 MHz
 */
#define DETWINDOW_VAL		0xa28

/*
 * mili Volt to config value. voltage = 700mV + val * 6.25mV
 * val = (voltage - 700) / 6.25
 * @mV:	mili volt
 */
#define MV_TO_VAL(MV)		((((MV) - 700) * 100 + 625 - 1) / 625) // TODO: FIXME, refer to VOLT_TO_PMIC_VAL()
#define VAL_TO_MV(VAL)		(((VAL) * 625) / 100 + 600) // TODO: FIXME, refer to PMIC_VAL_TO_VOLT()

#define VMAX_VAL		MV_TO_VAL(1150)
#define VMIN_VAL		MV_TO_VAL(1050) // XXX: for VCORE_AO & VCORE_PDN

#define DTHI_VAL		0x01		/* positive */
#define DTLO_VAL		0xfe		/* negative (2's compliment) */
#define DETMAX_VAL		0xffff		/* This timeout value is in cycles of bclk_ck. */
#define AGECONFIG_VAL		0x555555	/* FIXME */
#define AGEM_VAL		0x0		/* FIXME */
#define DVTFIXED_VAL		0X4		/* FIXME */
#define VCO_VAL			0X38		/* FIXME */
#define DCCONFIG_VAL		0x555555	/* FIXME */

/*
 * bit operation
 */
#undef  BIT
#define BIT(bit)	(1U << (bit))

#define MSB(range)	(1 ? range)
#define LSB(range)	(0 ? range)
/**
 * Genearte a mask wher MSB to LSB are all 0b1
 * @r:	Range in the form of MSB:LSB
 */
#define BITMASK(r)	(((unsigned) -1 >> (31 - MSB(r))) & ~((1U << LSB(r)) - 1))

/**
 * Set value at MSB:LSB. For example, BITS(7:3, 0x5A)
 * will return a value where bit 3 to bit 7 is 0x5A
 * @r:	Range in the form of MSB:LSB
 */
/* BITS(MSB:LSB, value) => Set value at MSB:LSB  */
#define BITS(r, val)	((val << LSB(r)) & BITMASK(r))

/**
 * iterate over list of detectors
 * @det:	the detector * to use as a loop cursor.
 */
#define for_each_det(det) for (det = ptp_detectors; det < (ptp_detectors + ARRAY_SIZE(ptp_detectors)); det++)

/**
 * Given a ptp_det * in ptp_detectors. Return the id.
 * @det:	pointer to a ptp_det in ptp_detectors
 */
#define det_to_id(det)	((det) - &ptp_detectors[0])

/*
 * LOG
 */
#define ptp_emerg(fmt, args...)     printf("[PTP] " fmt, ##args)
#define ptp_alert(fmt, args...)     printf("[PTP] " fmt, ##args)
#define ptp_crit(fmt, args...)      printf("[PTP] " fmt, ##args)
#define ptp_error(fmt, args...)     printf("[PTP] " fmt, ##args)
#define ptp_warning(fmt, args...)   printf("[PTP] " fmt, ##args)
#define ptp_notice(fmt, args...)    printf("[PTP] " fmt, ##args)
#define ptp_info(fmt, args...)      printf("[PTP] " fmt, ##args)
#define ptp_debug(fmt, args...)     printf("[PTP] " fmt, ##args)

#if EN_ISR_LOG
#define ptp_isr_info(fmt, args...)  ptp_notice(fmt, ##args)
#else
#define ptp_isr_info(fmt, args...)  ptp_debug(fmt, ##args)
#endif

#define FUNC_LV_MODULE          BIT(0)  /* module, platform driver interface */
#define FUNC_LV_CPUFREQ         BIT(1)  /* cpufreq driver interface          */
#define FUNC_LV_API             BIT(2)  /* mt_cpufreq driver global function */
#define FUNC_LV_LOCAL           BIT(3)  /* mt_cpufreq driver lcaol function  */
#define FUNC_LV_HELP            BIT(4)  /* mt_cpufreq driver help function   */

static unsigned int func_lv_mask = (FUNC_LV_MODULE | FUNC_LV_CPUFREQ | FUNC_LV_API | FUNC_LV_LOCAL | FUNC_LV_HELP);

#if defined(CONFIG_PTP_SHOWLOG)
#define FUNC_ENTER(lv)          do { if ((lv) & func_lv_mask) ptp_debug(">> %s()\n", __func__); } while (0)
#define FUNC_EXIT(lv)           do { if ((lv) & func_lv_mask) ptp_debug("<< %s():%d\n", __func__, __LINE__); } while (0)
#else
#define FUNC_ENTER(lv)
#define FUNC_EXIT(lv)
#endif /* CONFIG_CPU_DVFS_SHOWLOG */

/*
 * REG ACCESS
 */
#define ptp_read(addr)				(*(volatile unsigned int *)(addr))
#define ptp_read_field(addr, range)		((ptp_read(addr) & BITMASK(range)) >> LSB(range))
#define ptp_write(addr, val)			do { (*(volatile unsigned int *)(addr) = (unsigned int)(val)); } while(0)
#define ptp_write_field(addr, range, val)	ptp_write(addr, (ptp_read(addr) & ~BITMASK(range)) | BITS(range, val))


/*=============================================================
 * Local type definition
 *=============================================================*/

typedef enum {
	PTP_PHASE_INIT01,
	PTP_PHASE_INIT02,
	PTP_PHASE_MON,

	NR_PTP_PHASE,
} ptp_phase;

enum ptp_features {
	FEA_INIT01	= BIT(PTP_PHASE_INIT01),
	FEA_INIT02	= BIT(PTP_PHASE_INIT02),
	FEA_MON		= BIT(PTP_PHASE_MON),
};

struct ptp_devinfo {
	/* M_HW_RES0 10206180*/
	unsigned int CPU_BDES: 8;
	unsigned int CPU_MDES: 8;
	unsigned int CPU_DCBDET: 8;
	unsigned int CPU_DCMDET: 8;
	/* M_HW_RES1 10206184*/
	unsigned int GPU_MTDES: 8;
	unsigned int GPU_AGEDELTA: 8;
	unsigned int CPU_MTDES: 8;
	unsigned int CPU_AGEDELTA: 8;
	/* M_HW_RES2 10206188*/
#if defined(MACH_TYPE_MT6735) //D1
	unsigned int SOC_VOLTBIN: 2;
	unsigned int LTE_VOLTBIN: 2;
	unsigned int GPU_BDES: 4;
#else //D2
	unsigned int SOC_VOLTBIN: 2;
	unsigned int LTE_VOLTBIN: 2;
	unsigned int SOC_VOLTBIN_550: 2;
	unsigned int GPU_BDES: 2;
#endif
	unsigned int GPU_MDES: 8;
	unsigned int GPU_DCBDET: 8;
	unsigned int GPU_DCMDET: 8;
	/* M_HW_RES3 1020618C*/
	unsigned int M_HW_RES3_1: 32;
	/* M_HW_RES4 10206190*/
	unsigned int LTE_BDES: 8;
	unsigned int LTE_MDES: 8;
	unsigned int LTE_DCBDET: 8;
	unsigned int LTE_DCMDET: 8;
	/* M_HW_RES5 10206194*/
	unsigned int PTPINITEN: 1;
	unsigned int PTPMONEN: 1;
	unsigned int Bodybias: 1;
	unsigned int PTPOD_T: 1;
	unsigned int EPS: 1;
	unsigned int M_HW_RES5_OTHERS: 11;
	unsigned int LTE_MTDES: 8;
	unsigned int LTE_AGEDELTA: 8;
};

struct ptp_det {
	const char *name;
	// struct ptp_det_ops *ops;
	int status;		/* TODO: enable/disable */
	int features;		/* enum ptp_features */
	ptp_ctrl_id ctrl_id;

	/* devinfo */
	unsigned int PTPINITEN;
	unsigned int PTPMONEN;
	unsigned int MDES;
	unsigned int BDES;
	unsigned int DCMDET;
	unsigned int DCBDET;
	unsigned int AGEDELTA;
	unsigned int MTDES;

	/* constant */
	unsigned int DETWINDOW;
	unsigned int VMAX;
	unsigned int VMIN;
	unsigned int DTHI;
	unsigned int DTLO;
	unsigned int VBOOT;
	unsigned int DETMAX;
	unsigned int AGECONFIG;
	unsigned int AGEM;
	unsigned int DVTFIXED;
	unsigned int VCO;
	unsigned int DCCONFIG;

	/* Generated by PTP init01. Used in PTP init02 */
	unsigned int DCVOFFSETIN;
	unsigned int AGEVOFFSETIN;

	/* slope */
	unsigned int MTS;
	unsigned int BTS;

	/* dvfs */
	unsigned int num_freq_tbl; /* could be got @ the same time
				      with freq_tbl[] */
	unsigned int max_freq_khz; /* maximum frequency used to
				      calculate percentage */
	unsigned char freq_tbl[NR_FREQ]; /* percentage to maximum freq */

	unsigned int volt_tbl[NR_FREQ];
	unsigned int volt_tbl_init2[NR_FREQ];
	unsigned int volt_tbl_pmic[NR_FREQ];
	unsigned int volt_tbl_bin[NR_FREQ];
	int volt_offset;

	int disabled; /* Disabled by error or sysfs */
};


/*=============================================================
 *Local variable definition
 *=============================================================*/

static struct ptp_devinfo ptp_devinfo;

static struct ptp_det ptp_detectors[NR_PTP_DET] = {
	[PTP_DET_SOC] = {
		.name		= __stringify(PTP_DET_SOC),
		// .ops		= &vcore_pdn_det_ops,
		.ctrl_id	= PTP_CTRL_SOC,
		.features	= FEA_INIT01 | FEA_INIT02,
		.VBOOT		= MV_TO_VAL(1150), /* 1.0v: 0x30 */
	},
};


/*=============================================================
 * Local function definition
 *=============================================================*/
const static struct devinfo {
	int sn;
	int M_HW_RES4;
	int M_HW_RES5;
	int M_HW_RES3_1;
	int M_HW_RES0;
	int M_HW_RES1;
	int M_HW_RES7;
	int M_HW_RES8;
	int M_HW_RES9;
	int M_HW_RES6;
	int core;
	int gpu;
	int sram2;
	int sram1;
} devinfo[] = {
	{ 1410, 1401856, 637949991, 0x253013CF, 0x13131313, 0xCFCECFC9, 0x192C3C40, 0x43230A0C, 0x3B3C5A51, 0x76414681, 88.155296, 25.598700, 26.679930, 107.494598 },
};

static void get_devinfo(struct ptp_devinfo *p)
{
	int *val = (int *)p;
	int i;

	FUNC_ENTER(FUNC_LV_HELP);

	val[0] = seclib_get_devinfo_with_index(7); // ptp_read(0x10206180);	/* M_HW_RES0 */
	val[1] = seclib_get_devinfo_with_index(8); // ptp_read(0x10206184);	/* M_HW_RES1 */
	val[2] = seclib_get_devinfo_with_index(9); // ptp_read(0x10206188);	/* M_HW_RES2 */
	val[3] = seclib_get_devinfo_with_index(14); // ptp_read(0x1020618C);	/* M_HW_RES3 */
	val[4] = seclib_get_devinfo_with_index(15); // ptp_read(0x10206190);	/* M_HW_RES4 */
	val[5] = seclib_get_devinfo_with_index(16); // ptp_read(0x10206194);	/* M_HW_RES5 */
#if 0
	if (0 == p->PTPINITEN) {

		for (i = 0; i < ARRAY_SIZE(devinfo); i++) {

			if (val[4] == devinfo[i].M_HW_RES4 && val[5] == devinfo[i].M_HW_RES5) {
				val[0] = devinfo[i].M_HW_RES0;			/* M_HW_RES0 */
				val[1] = devinfo[i].M_HW_RES1;			/* M_HW_RES1 */
				val[2] = seclib_get_devinfo_with_index(9);	/* M_HW_RES2 */
				val[3] = devinfo[i].M_HW_RES3_1;			/* M_HW_RES3 */
				val[6] = devinfo[i].M_HW_RES6;			/* M_HW_RES6 */
				val[7] = devinfo[i].M_HW_RES7;			/* M_HW_RES7 */
				val[8] = devinfo[i].M_HW_RES8;			/* M_HW_RES8 */
				val[9] = devinfo[i].M_HW_RES9;			/* M_HW_RES9 */

				p->PTPINITEN = 1; // TODO: FIXME
				p->PTPMONEN  = 1; // TODO: FIXME

				break;
			}
		}
	}
#endif
	FUNC_EXIT(FUNC_LV_HELP);
}
#define PTPOD_PMIC_OFFSET (0x10)
unsigned int have_550 = 0;
static void ptp_init_det(struct ptp_det *det, struct ptp_devinfo *devinfo)
{
	ptp_det_id det_id = det_to_id(det);

	FUNC_ENTER(FUNC_LV_HELP);

	/* init with devinfo */
	det->PTPINITEN		= devinfo->PTPINITEN;
	det->PTPMONEN		= devinfo->PTPMONEN;

	/* init with constant */
	det->DETWINDOW	= DETWINDOW_VAL;
	det->VMAX	= VMAX_VAL;
	det->VMIN	= VMIN_VAL;

	det->DTHI	= DTHI_VAL;
	det->DTLO	= DTLO_VAL;
	det->DETMAX	= DETMAX_VAL;

	det->AGECONFIG	= AGECONFIG_VAL;
	det->AGEM	= AGEM_VAL;
	det->DVTFIXED	= DVTFIXED_VAL;
	det->VCO	= VCO_VAL;
	det->DCCONFIG	= DCCONFIG_VAL;

	switch (det_id) {
	case PTP_DET_SOC:
		det->MDES	= devinfo->GPU_MDES;
		det->BDES	= devinfo->GPU_BDES;
		det->DCMDET	= devinfo->GPU_DCMDET;
		det->DCBDET	= devinfo->GPU_DCBDET;
		//det->VBOOT	= MV_TO_VAL(1000);
		det->VBOOT	= upmu_get_reg_value(0x618); //This should be 1.15V 0x48

#if defined(MACH_TYPE_MT6735M) //D2
		switch (devinfo->SOC_VOLTBIN_550) {
			case 0:
				have_550 = 0;
				det->volt_tbl_bin[0] = MV_TO_VAL(1150) + PTPOD_PMIC_OFFSET;
				break;
			case 1:
				have_550 = 1;
				det->volt_tbl_bin[0] = MV_TO_VAL(1200) + PTPOD_PMIC_OFFSET;
				break;
			case 2:
				have_550 = 1;
				det->volt_tbl_bin[0] = MV_TO_VAL(1150) + PTPOD_PMIC_OFFSET;
				break;
			case 3:
				have_550 = 1;
				det->volt_tbl_bin[0] = MV_TO_VAL(1250) + PTPOD_PMIC_OFFSET;
				break;
		}
#endif
		switch (devinfo->SOC_VOLTBIN) {
			case 0:
				ptp_isr_info("VCORE voltage bin to 1.5V\n");
				det->volt_tbl_bin[1] = MV_TO_VAL(1150) + PTPOD_PMIC_OFFSET;
				det->volt_tbl_bin[2] = MV_TO_VAL(1050) + PTPOD_PMIC_OFFSET;
				break;

			case 1:
				ptp_isr_info("VCORE voltage bin to 1.1V\n");
				det->volt_tbl_bin[1] = MV_TO_VAL(1100) + PTPOD_PMIC_OFFSET;
				det->volt_tbl_bin[2] = MV_TO_VAL(1050) + PTPOD_PMIC_OFFSET;
				break;

			case 2:
				ptp_isr_info("VCORE voltage bin to 1.05V\n");
				det->volt_tbl_bin[1] = MV_TO_VAL(1050) + PTPOD_PMIC_OFFSET;
				det->volt_tbl_bin[2] = MV_TO_VAL(1050) + PTPOD_PMIC_OFFSET;
				break;

			default:
				det->volt_tbl_bin[1] = MV_TO_VAL(1150) + PTPOD_PMIC_OFFSET;
				det->volt_tbl_bin[2] = MV_TO_VAL(1050) + PTPOD_PMIC_OFFSET;
				break;
		};
		/* get DVFS frequency table */
		//det->num_freq_tbl = 3;
		//det->freq_tbl[0] = 1600 * 100 / 1600; // XXX: percentage, 800/800, dedicated for VCORE only
		//det->freq_tbl[1] = 1333 * 100 / 1600; // XXX: percentage, 600/800, dedicated for VCORE only
		//det->freq_tbl[2] = 800 * 100 / 1600; // XXX: percentage, 600/800, dedicated for VCORE only

		break;

	default:
		ptp_error("[%s]: Unknown det_id %d\n", __func__, det_id);
		break;
	}

	switch (det->ctrl_id) {
	case PTP_CTRL_CPU:
		det->MDES	= devinfo->CPU_MDES;
		det->BDES	= devinfo->CPU_BDES;
		det->DCMDET	= devinfo->CPU_DCMDET;
		det->DCBDET	= devinfo->CPU_DCBDET;
		break;

	case PTP_CTRL_SOC:
		det->MDES	= devinfo->GPU_MDES;
		det->BDES	= devinfo->GPU_BDES;
		det->DCMDET	= devinfo->GPU_DCMDET;
		det->DCBDET	= devinfo->GPU_DCBDET;
		break;

	case PTP_CTRL_LTE:
		det->MDES	= devinfo->LTE_MDES;
		det->BDES	= devinfo->LTE_BDES;
		det->DCMDET	= devinfo->LTE_DCMDET;
		det->DCBDET	= devinfo->LTE_DCBDET;
		break;

	default:
		ptp_error("[%s]: Unknown ctrl_id %d\n", __func__, det->ctrl_id);
		break;
	}

	FUNC_EXIT(FUNC_LV_HELP);
}

static void base_ops_set_phase(struct ptp_det *det, ptp_phase phase)
{
	unsigned int i, filter, val;
	// unsigned long flags; // <-XXX

	FUNC_ENTER(FUNC_LV_HELP);

	// mt_ptp_lock(&flags); // <-XXX

#if 0
	det->ops->switch_bank(det);
#else//suppose to be PDN
	/*ptp_write_field(PERI_VCORE_PTPOD_CON0, VCORE_PTPODSEL, (det == &ptp_detectors[PTP_DET_VCORE_AO]) ? SEL_VCORE_AO : SEL_VCORE_PDN);
	ptp_write_field(PTP_PTPCORESEL, APBSEL, det->ctrl_id);*/
	ptp_write_field(PTP_PTPCORESEL, APBSEL, PTP_CTRL_SOC);
#endif
	/* config PTP register */
	ptp_write(PTP_DESCHAR, ((det->BDES << 8) & 0xff00) | (det->MDES & 0xff));
	ptp_write(PTP_TEMPCHAR, (((det->VCO << 16) & 0xff0000) | ((det->MTDES << 8) & 0xff00) | (det->DVTFIXED & 0xff)));
	ptp_write(PTP_DETCHAR, ((det->DCBDET << 8) & 0xff00) | (det->DCMDET & 0xff));
	ptp_write(PTP_AGECHAR, ((det->AGEDELTA << 8) & 0xff00) | (det->AGEM & 0xff));
	ptp_write(PTP_DCCONFIG, det->DCCONFIG);
	ptp_write(PTP_AGECONFIG, det->AGECONFIG);

	if (PTP_PHASE_MON == phase)
		ptp_write(PTP_TSCALCS, ((det->BTS << 12) & 0xfff000) | (det->MTS & 0xfff));

	if (det->AGEM == 0x0)
		ptp_write(PTP_RUNCONFIG, 0x80000000);
	else {
		val = 0x0;

		for (i = 0; i < 24; i += 2) {
			filter = 0x3 << i;

			if (((det->AGECONFIG) & filter) == 0x0)
				val |= (0x1 << i);
			else
				val |= ((det->AGECONFIG) & filter);
		}

		ptp_write(PTP_RUNCONFIG, val);
	}

	ptp_write(PTP_FREQPCT30,
		  ((det->freq_tbl[3] << 24) & 0xff000000)	|
		  ((det->freq_tbl[2] << 16) & 0xff0000)	|
		  ((det->freq_tbl[1] << 8) & 0xff00)	|
		  (det->freq_tbl[0] & 0xff));
	ptp_write(PTP_FREQPCT74,
		  ((det->freq_tbl[7] << 24) & 0xff000000)	|
		  ((det->freq_tbl[6] << 16) & 0xff0000)	|
		  ((det->freq_tbl[5] << 8) & 0xff00)	|
		  ((det->freq_tbl[4]) & 0xff));
	ptp_write(PTP_LIMITVALS,
		  ((det->VMAX << 24) & 0xff000000)	|
		  ((det->VMIN << 16) & 0xff0000)		|
		  ((det->DTHI << 8) & 0xff00)		|
		  (det->DTLO & 0xff));
	ptp_write(PTP_VBOOT, (((det->VBOOT) & 0xff)));
	ptp_write(PTP_DETWINDOW, (((det->DETWINDOW) & 0xffff)));
	ptp_write(PTP_PTPCONFIG, (((det->DETMAX) & 0xffff)));

	/* clear all pending PTP interrupt & config PTPINTEN */
	ptp_write(PTP_PTPINTSTS, 0xffffffff);

	switch (phase) {
	case PTP_PHASE_INIT01:
		ptp_write(PTP_PTPINTEN, 0x00005f01);
		/* enable PTP INIT measurement */
		ptp_write(PTP_PTPEN, 0x00000001);
		break;

	case PTP_PHASE_INIT02:
		ptp_write(PTP_PTPINTEN, 0x00005f01);
		ptp_write(PTP_INIT2VALS,
			  ((det->AGEVOFFSETIN << 16) & 0xffff0000) |
			  (det->DCVOFFSETIN & 0xffff));
		/* enable PTP INIT measurement */
		ptp_write(PTP_PTPEN, 0x00000005);
		break;

	case PTP_PHASE_MON:
		ptp_write(PTP_PTPINTEN, 0x00FF0000);
		/* enable PTP monitor mode */
		ptp_write(PTP_PTPEN, 0x00000002);
		break;

	default:
		ptp_error("[%s]: Unknown phase %d\n", __func__, phase);
		break;
	}

	// mt_ptp_unlock(&flags); // <-XXX

	FUNC_EXIT(FUNC_LV_HELP);
}

static void mt_ptp_reg_dump_locked(void)
{
	unsigned int addr;

	for (addr = PTP_DESCHAR; addr <= PTP_SMSTATE1; addr += 4)
		ptp_isr_info("%x = %x\n", addr, *(volatile unsigned int *)addr);

	addr = PTP_PTPCORESEL;
	ptp_isr_info("%x = %x\n", addr, *(volatile unsigned int *)addr);
}

static void mt_ptp_reg_dump(void)
{
	struct ptp_det *det;
	unsigned long flags;

	FUNC_ENTER(FUNC_LV_HELP);
#if 0
	ptp_isr_info("PTP_REVISIONID	= %x\n", ptp_read(PTP_REVISIONID));
	ptp_isr_info("PTP_TEMPMONCTL0	= %x\n", ptp_read(PTP_TEMPMONCTL0));
	ptp_isr_info("PTP_TEMPMONCTL1	= %x\n", ptp_read(PTP_TEMPMONCTL1));
	ptp_isr_info("PTP_TEMPMONCTL2	= %x\n", ptp_read(PTP_TEMPMONCTL2));
	ptp_isr_info("PTP_TEMPMONINT	= %x\n", ptp_read(PTP_TEMPMONINT));
	ptp_isr_info("PTP_TEMPMONINTSTS	= %x\n", ptp_read(PTP_TEMPMONINTSTS));
	ptp_isr_info("PTP_TEMPMONIDET0	= %x\n", ptp_read(PTP_TEMPMONIDET0));
	ptp_isr_info("PTP_TEMPMONIDET1	= %x\n", ptp_read(PTP_TEMPMONIDET1));
	ptp_isr_info("PTP_TEMPMONIDET2	= %x\n", ptp_read(PTP_TEMPMONIDET2));
	ptp_isr_info("PTP_TEMPH2NTHRE	= %x\n", ptp_read(PTP_TEMPH2NTHRE));
	ptp_isr_info("PTP_TEMPHTHRE	= %x\n", ptp_read(PTP_TEMPHTHRE));
	ptp_isr_info("PTP_TEMPCTHRE	= %x\n", ptp_read(PTP_TEMPCTHRE));
	ptp_isr_info("PTP_TEMPOFFSETH	= %x\n", ptp_read(PTP_TEMPOFFSETH));
	ptp_isr_info("PTP_TEMPOFFSETL	= %x\n", ptp_read(PTP_TEMPOFFSETL));
	ptp_isr_info("PTP_TEMPMSRCTL0	= %x\n", ptp_read(PTP_TEMPMSRCTL0));
	ptp_isr_info("PTP_TEMPMSRCTL1	= %x\n", ptp_read(PTP_TEMPMSRCTL1));
	ptp_isr_info("PTP_TEMPAHBPOLL	= %x\n", ptp_read(PTP_TEMPAHBPOLL));
	ptp_isr_info("PTP_TEMPAHBTO	= %x\n", ptp_read(PTP_TEMPAHBTO));
	ptp_isr_info("PTP_TEMPADCPNP0	= %x\n", ptp_read(PTP_TEMPADCPNP0));
	ptp_isr_info("PTP_TEMPADCPNP1	= %x\n", ptp_read(PTP_TEMPADCPNP1));
	ptp_isr_info("PTP_TEMPADCPNP2	= %x\n", ptp_read(PTP_TEMPADCPNP2));
	ptp_isr_info("PTP_TEMPADCMUX	= %x\n", ptp_read(PTP_TEMPADCMUX));
	ptp_isr_info("PTP_TEMPADCEXT	= %x\n", ptp_read(PTP_TEMPADCEXT));
	ptp_isr_info("PTP_TEMPADCEXT1	= %x\n", ptp_read(PTP_TEMPADCEXT1));
	ptp_isr_info("PTP_TEMPADCEN	= %x\n", ptp_read(PTP_TEMPADCEN));
	ptp_isr_info("PTP_TEMPPNPMUXADDR	= %x\n", ptp_read(PTP_TEMPPNPMUXADDR));
	ptp_isr_info("PTP_TEMPADCMUXADDR	= %x\n", ptp_read(PTP_TEMPADCMUXADDR));
	ptp_isr_info("PTP_TEMPADCEXTADDR	= %x\n", ptp_read(PTP_TEMPADCEXTADDR));
	ptp_isr_info("PTP_TEMPADCEXT1ADDR	= %x\n", ptp_read(PTP_TEMPADCEXT1ADDR));
	ptp_isr_info("PTP_TEMPADCENADDR	= %x\n", ptp_read(PTP_TEMPADCENADDR));
	ptp_isr_info("PTP_TEMPADCVALIDADDR	= %x\n", ptp_read(PTP_TEMPADCVALIDADDR));
	ptp_isr_info("PTP_TEMPADCVOLTADDR	= %x\n", ptp_read(PTP_TEMPADCVOLTADDR));
	ptp_isr_info("PTP_TEMPRDCTRL	= %x\n", ptp_read(PTP_TEMPRDCTRL));
	ptp_isr_info("PTP_TEMPADCVALIDMASK	= %x\n", ptp_read(PTP_TEMPADCVALIDMASK));
	ptp_isr_info("PTP_TEMPADCVOLTAGESHIFT	= %x\n", ptp_read(PTP_TEMPADCVOLTAGESHIFT));
	ptp_isr_info("PTP_TEMPADCWRITECTRL	= %x\n", ptp_read(PTP_TEMPADCWRITECTRL));
	ptp_isr_info("PTP_TEMPMSR0	= %x\n", ptp_read(PTP_TEMPMSR0));
	ptp_isr_info("PTP_TEMPMSR1	= %x\n", ptp_read(PTP_TEMPMSR1));
	ptp_isr_info("PTP_TEMPMSR2	= %x\n", ptp_read(PTP_TEMPMSR2));
	ptp_isr_info("PTP_TEMPIMMD0	= %x\n", ptp_read(PTP_TEMPIMMD0));
	ptp_isr_info("PTP_TEMPIMMD1	= %x\n", ptp_read(PTP_TEMPIMMD1));
	ptp_isr_info("PTP_TEMPIMMD2	= %x\n", ptp_read(PTP_TEMPIMMD2));
	ptp_isr_info("PTP_TEMPMONIDET3	= %x\n", ptp_read(PTP_TEMPMONIDET3));
	ptp_isr_info("PTP_TEMPADCPNP3	= %x\n", ptp_read(PTP_TEMPADCPNP3));
	ptp_isr_info("PTP_TEMPMSR3	= %x\n", ptp_read(PTP_TEMPMSR3));
	ptp_isr_info("PTP_TEMPIMMD3	= %x\n", ptp_read(PTP_TEMPIMMD3));
	ptp_isr_info("PTP_TEMPPROTCTL	= %x\n", ptp_read(PTP_TEMPPROTCTL));
	ptp_isr_info("PTP_TEMPPROTTA	= %x\n", ptp_read(PTP_TEMPPROTTA));
	ptp_isr_info("PTP_TEMPPROTTB	= %x\n", ptp_read(PTP_TEMPPROTTB));
	ptp_isr_info("PTP_TEMPPROTTC	= %x\n", ptp_read(PTP_TEMPPROTTC));
	ptp_isr_info("PTP_TEMPSPARE0	= %x\n", ptp_read(PTP_TEMPSPARE0));
	ptp_isr_info("PTP_TEMPSPARE1	= %x\n", ptp_read(PTP_TEMPSPARE1));
	ptp_isr_info("PTP_TEMPSPARE2	= %x\n", ptp_read(PTP_TEMPSPARE2));
	ptp_isr_info("PTP_TEMPSPARE3	= %x\n", ptp_read(PTP_TEMPSPARE3));
#endif
	for_each_det(det)
	{
		// mt_ptp_lock(&flags);
#if 0
		det->ops->switch_bank(det);
#else
		/*ptp_write_field(PERI_VCORE_PTPOD_CON0, VCORE_PTPODSEL, (det == &ptp_detectors[PTP_DET_VCORE_AO]) ? SEL_VCORE_AO : SEL_VCORE_PDN);
		ptp_write_field(PTP_PTPCORESEL, APBSEL, det->ctrl_id);*/
		ptp_write_field(PTP_PTPCORESEL, APBSEL, PTP_CTRL_SOC);
#endif

#if 0
		ptp_isr_info("PTP_DESCHAR[%s]	= %x\n", det->name, ptp_read(PTP_DESCHAR));
		ptp_isr_info("PTP_TEMPCHAR[%s]	= %x\n", det->name, ptp_read(PTP_TEMPCHAR));
		ptp_isr_info("PTP_DETCHAR[%s]	= %x\n", det->name, ptp_read(PTP_DETCHAR));
		ptp_isr_info("PTP_AGECHAR[%s]	= %x\n", det->name, ptp_read(PTP_AGECHAR));
		ptp_isr_info("PTP_DCCONFIG[%s]	= %x\n", det->name, ptp_read(PTP_DCCONFIG));
		ptp_isr_info("PTP_AGECONFIG[%s]	= %x\n", det->name, ptp_read(PTP_AGECONFIG));
		ptp_isr_info("PTP_FREQPCT30[%s]	= %x\n", det->name, ptp_read(PTP_FREQPCT30));
		ptp_isr_info("PTP_FREQPCT74[%s]	= %x\n", det->name, ptp_read(PTP_FREQPCT74));
		ptp_isr_info("PTP_LIMITVALS[%s]	= %x\n", det->name, ptp_read(PTP_LIMITVALS));
		ptp_isr_info("PTP_VBOOT[%s]	= %x\n", det->name, ptp_read(PTP_VBOOT));
		ptp_isr_info("PTP_DETWINDOW[%s]	= %x\n", det->name, ptp_read(PTP_DETWINDOW));
		ptp_isr_info("PTP_PTPCONFIG[%s]	= %x\n", det->name, ptp_read(PTP_PTPCONFIG));
		ptp_isr_info("PTP_TSCALCS[%s]	= %x\n", det->name, ptp_read(PTP_TSCALCS));
		ptp_isr_info("PTP_RUNCONFIG[%s]	= %x\n", det->name, ptp_read(PTP_RUNCONFIG));
		ptp_isr_info("PTP_PTPEN[%s]	= %x\n", det->name, ptp_read(PTP_PTPEN));
		ptp_isr_info("PTP_INIT2VALS[%s]	= %x\n", det->name, ptp_read(PTP_INIT2VALS));
		ptp_isr_info("PTP_DCVALUES[%s]	= %x\n", det->name, ptp_read(PTP_DCVALUES));
		ptp_isr_info("PTP_AGEVALUES[%s]	= %x\n", det->name, ptp_read(PTP_AGEVALUES));
		ptp_isr_info("PTP_VOP30[%s]	= %x\n", det->name, ptp_read(PTP_VOP30));
		ptp_isr_info("PTP_VOP74[%s]	= %x\n", det->name, ptp_read(PTP_VOP74));
		ptp_isr_info("PTP_TEMP[%s]	= %x\n", det->name, ptp_read(PTP_TEMP));
		ptp_isr_info("PTP_PTPINTSTS[%s]	= %x\n", det->name, ptp_read(PTP_PTPINTSTS));
		ptp_isr_info("PTP_PTPINTSTSRAW[%s]	= %x\n", det->name, ptp_read(PTP_PTPINTSTSRAW));
		ptp_isr_info("PTP_PTPINTEN[%s]	= %x\n", det->name, ptp_read(PTP_PTPINTEN));
		ptp_isr_info("PTP_SMSTATE0[%s]	= %x\n", det->name, ptp_read(PTP_SMSTATE0));
		ptp_isr_info("PTP_SMSTATE1[%s]	= %x\n", det->name, ptp_read(PTP_SMSTATE1));
#else
		ptp_isr_info("PTP_240[%s]	= %x\n", det->name, ptp_read(PTP_DCVALUES));
		ptp_isr_info("PTP_218[%s]	= %x\n", det->name, ptp_read(PTP_FREQPCT30));
		ptp_isr_info("PTP_26C[%s]	= %x\n", det->name, ptp_read(PTP_PTPINTEN+0x10));
		ptp_isr_info("PTP_248[%s]	= %x\n", det->name, ptp_read(PTP_VOP30));
		ptp_isr_info("PTP_238[%s]	= %x\n", det->name, ptp_read(PTP_PTPEN));
#endif
		// mt_ptp_unlock(&flags);
	}
#if 0
	ptp_isr_info("PTP_PTPCORESEL	= %x\n", ptp_read(PTP_PTPCORESEL));
	ptp_isr_info("PTP_THERMINTST	= %x\n", ptp_read(PTP_THERMINTST));
	ptp_isr_info("PTP_PTPODINTST	= %x\n", ptp_read(PTP_PTPODINTST));
	ptp_isr_info("PTP_THSTAGE0ST	= %x\n", ptp_read(PTP_THSTAGE0ST));
	ptp_isr_info("PTP_THSTAGE1ST	= %x\n", ptp_read(PTP_THSTAGE1ST));
	ptp_isr_info("PTP_THSTAGE2ST	= %x\n", ptp_read(PTP_THSTAGE2ST));
	ptp_isr_info("PTP_THAHBST0	= %x\n", ptp_read(PTP_THAHBST0));
	ptp_isr_info("PTP_THAHBST1	= %x\n", ptp_read(PTP_THAHBST1));
	ptp_isr_info("PTP_PTPSPARE0	= %x\n", ptp_read(PTP_PTPSPARE0));
	ptp_isr_info("PTP_PTPSPARE1	= %x\n", ptp_read(PTP_PTPSPARE1));
	ptp_isr_info("PTP_PTPSPARE2	= %x\n", ptp_read(PTP_PTPSPARE2));
	ptp_isr_info("PTP_PTPSPARE3	= %x\n", ptp_read(PTP_PTPSPARE3));
	ptp_isr_info("PTP_THSLPEVEB	= %x\n", ptp_read(PTP_THSLPEVEB));
#endif
	FUNC_EXIT(FUNC_LV_HELP);
}

static inline void handle_init01_isr(struct ptp_det *det)
{
	FUNC_ENTER(FUNC_LV_LOCAL);

	ptp_isr_info("@ %s(%s)\n", __func__, det->name);

	/*
	 * Read & store 16 bit values DCVALUES.DCVOFFSET and
	 * AGEVALUES.AGEVOFFSET for later use in INIT2 procedure
	 */
	det->DCVOFFSETIN = ~(ptp_read(PTP_DCVALUES) & 0xffff) + 1; /* hw bug, workaround */
	det->AGEVOFFSETIN = ptp_read(PTP_AGEVALUES) & 0xffff;

	/*
	 * Set PTPEN.PTPINITEN/PTPEN.PTPINIT2EN = 0x0 &
	 * Clear PTP INIT interrupt PTPINTSTS = 0x00000001
	 */
	ptp_write(PTP_PTPEN, 0x0);
	ptp_write(PTP_PTPINTSTS, 0x1);
	// ptp_init01_finish(det);
	// base_ops_set_phase(det, PTP_PHASE_INIT02);

	FUNC_EXIT(FUNC_LV_LOCAL);
}

static inline void handle_init02_isr(struct ptp_det *det)
{
	unsigned int temp;
	int i;
	// struct ptp_ctrl *ctrl = id_to_ptp_ctrl(det->ctrl_id);

	FUNC_ENTER(FUNC_LV_LOCAL);

	ptp_isr_info("@ %s(%s)\n", __func__, det->name);

	temp = ptp_read(PTP_VOP30);
	det->volt_tbl[0] = temp & 0xff;
	det->volt_tbl[1] = (temp >> 8) & 0xff;
	det->volt_tbl[2] = (temp >> 16) & 0xff;
	det->volt_tbl[3] = (temp >> 24) & 0xff;

	temp = ptp_read(PTP_VOP74);
	det->volt_tbl[4] = temp & 0xff;
	det->volt_tbl[5] = (temp >> 8) & 0xff;
	det->volt_tbl[6] = (temp >> 16) & 0xff;
	det->volt_tbl[7] = (temp >> 24) & 0xff;

	memcpy(det->volt_tbl_init2, det->volt_tbl, sizeof(det->volt_tbl_init2));
	ptp_write(PTP_VBOOT, ((det->volt_tbl_init2[1]) & 0xff)+det_to_id(det));

	for (i = 0; i < NR_FREQ; i++)
		ptp_isr_info("ptp_detectors[%s].volt_tbl[%d] = %x\n", det->name, i, det->volt_tbl[i]);

	// ptp_isr_info("ptp_level = %x\n", ptp_level);

	// ptp_set_ptp_volt(det);

	/*
	 * Set PTPEN.PTPINITEN/PTPEN.PTPINIT2EN = 0x0 &
	 * Clear PTP INIT interrupt PTPINTSTS = 0x00000001
	 */
	ptp_write(PTP_PTPEN, 0x0);
	ptp_write(PTP_PTPINTSTS, 0x1);

	// atomic_dec(&ctrl->in_init);
	// complete(&ctrl->init_done);
	// det->ops->mon_mode(det);

	FUNC_EXIT(FUNC_LV_LOCAL);
}

static inline void handle_init_err_isr(struct ptp_det *det)
{
	FUNC_ENTER(FUNC_LV_LOCAL);

	ptp_isr_info("====================================================\n");
	ptp_isr_info("PTP init err: PTPEN(%x) = %x, PTPINTSTS(%x) = %x\n",
		     PTP_PTPEN, ptp_read(PTP_PTPEN),
		     PTP_PTPINTSTS, ptp_read(PTP_PTPINTSTS));
	ptp_isr_info("PTP_SMSTATE0 (%x) = %x\n",
		     PTP_SMSTATE0, ptp_read(PTP_SMSTATE0));
	ptp_isr_info("PTP_SMSTATE1 (%x) = %x\n",
		     PTP_SMSTATE1, ptp_read(PTP_SMSTATE1));
	ptp_isr_info("====================================================\n");
#if 0
	// TODO: FIXME
	{
		struct ptp_ctrl *ctrl = id_to_ptp_ctrl(det->ctrl_id);
		atomic_dec(&ctrl->in_init);
		complete(&ctrl->init_done);
	}
	// TODO: FIXME

	det->ops->disable_locked(det, BY_INIT_ERROR);
#endif
	FUNC_EXIT(FUNC_LV_LOCAL);
}

static inline void handle_mon_mode_isr(struct ptp_det *det)
{
	unsigned int temp;
	int i;

	FUNC_ENTER(FUNC_LV_LOCAL);

	ptp_isr_info("@ %s(%s)\n", __func__, det->name);

	/* check if thermal sensor init completed? */
	temp = (ptp_read(PTP_TEMP) & 0xff);

	if ((temp > 0x4b) && (temp < 0xd3)) {
		ptp_isr_info("thermal sensor init has not been completed. "
			     "(temp = %x)\n", temp);
		goto out;
	}

	temp = ptp_read(PTP_VOP30);
	det->volt_tbl[0] = temp & 0xff;
	det->volt_tbl[1] = (temp >> 8) & 0xff;
	det->volt_tbl[2] = (temp >> 16) & 0xff;
	det->volt_tbl[3] = (temp >> 24) & 0xff;

	temp = ptp_read(PTP_VOP74);
	det->volt_tbl[4] = temp & 0xff;
	det->volt_tbl[5] = (temp >> 8) & 0xff;
	det->volt_tbl[6] = (temp >> 16) & 0xff;
	det->volt_tbl[7] = (temp >> 24) & 0xff;

	for (i = 0; i < NR_FREQ; i++)
		ptp_isr_info("ptp_detectors[%s].volt_tbl[%d] = %x\n", det->name, i, det->volt_tbl[i]);

	// ptp_isr_info("ptp_level = %x\n", ptp_level);
	ptp_isr_info("ISR : TEMPSPARE1 = %x\n", ptp_read(PTP_TEMPSPARE1));
	// ptp_set_ptp_volt(det);

out:
	/* Clear PTP INIT interrupt PTPINTSTS = 0x00ff0000 */
	ptp_write(PTP_PTPINTSTS, 0x00ff0000);

	FUNC_EXIT(FUNC_LV_LOCAL);
}

static inline void handle_mon_err_isr(struct ptp_det *det)
{
	FUNC_ENTER(FUNC_LV_LOCAL);

	/* PTP Monitor mode error handler */
	ptp_isr_info("====================================================\n");
	ptp_isr_info("PTP mon err: PTPEN(%x) = %x, PTPINTSTS(%x) = %x\n",
		     PTP_PTPEN, ptp_read(PTP_PTPEN),
		     PTP_PTPINTSTS, ptp_read(PTP_PTPINTSTS));
	ptp_isr_info("PTP_SMSTATE0 (%x) = %x\n",
		     PTP_SMSTATE0, ptp_read(PTP_SMSTATE0));
	ptp_isr_info("PTP_SMSTATE1 (%x) = %x\n",
		     PTP_SMSTATE1, ptp_read(PTP_SMSTATE1));
	ptp_isr_info("PTP_TEMP (%x) = %x\n",
		     PTP_TEMP, ptp_read(PTP_TEMP));
	ptp_isr_info("PTP_TEMPMSR0 (%x) = %x\n",
		     PTP_TEMPMSR0, ptp_read(PTP_TEMPMSR0));
	ptp_isr_info("PTP_TEMPMSR1 (%x) = %x\n",
		     PTP_TEMPMSR1, ptp_read(PTP_TEMPMSR1));
	ptp_isr_info("PTP_TEMPMSR2 (%x) = %x\n",
		     PTP_TEMPMSR2, ptp_read(PTP_TEMPMSR2));
	ptp_isr_info("PTP_TEMPMONCTL0 (%x) = %x\n",
		     PTP_TEMPMONCTL0, ptp_read(PTP_TEMPMONCTL0));
	ptp_isr_info("PTP_TEMPMSRCTL1 (%x) = %x\n",
		     PTP_TEMPMSRCTL1, ptp_read(PTP_TEMPMSRCTL1));
	ptp_isr_info("====================================================\n");

	// det->ops->disable_locked(det, BY_MON_ERROR);

	FUNC_EXIT(FUNC_LV_LOCAL);
}

static inline void ptp_isr_handler(struct ptp_det *det)
{
	unsigned int PTPINTSTS, PTPEN;

	FUNC_ENTER(FUNC_LV_LOCAL);

	PTPINTSTS = ptp_read(PTP_PTPINTSTS);
	PTPEN = ptp_read(PTP_PTPEN);

	ptp_isr_info("[%s]\n", det->name);
	ptp_isr_info("PTPINTSTS = %x\n", PTPINTSTS);
	ptp_isr_info("PTP_PTPEN = %x\n", PTPEN);
	ptp_isr_info("*(%x) = %x\n", PTP_DCVALUES, ptp_read(PTP_DCVALUES));
	ptp_isr_info("*(%x) = %x\n", PTP_AGECOUNT, ptp_read(PTP_AGECOUNT));

	if (PTPINTSTS == 0x1) { /* PTP init1 or init2 */
		if ((PTPEN & 0x7) == 0x1)   /* PTP init1 */
			handle_init01_isr(det);
		else if ((PTPEN & 0x7) == 0x5)   /* PTP init2 */
			handle_init02_isr(det);
		else {
			/*
			 * error : init1 or init2,
			 * but enable setting is wrong.
			 */
			handle_init_err_isr(det);
		}
	} else if ((PTPINTSTS & 0x00ff0000) != 0x0)
		handle_mon_mode_isr(det);
	else { /* PTP error handler */
		/* init 1  || init 2 error handler */
		if (((PTPEN & 0x7) == 0x1) || ((PTPEN & 0x7) == 0x5))
			handle_init_err_isr(det);
		else /* PTP Monitor mode error handler */
			handle_mon_err_isr(det);
	}

	FUNC_EXIT(FUNC_LV_LOCAL);
}

static void ptp_isr(void)
{
	struct ptp_det *det;

	FUNC_ENTER(FUNC_LV_MODULE);
#if 0
	while (BIT(PTP_CTRL_SOC) & ptp_read(PTP_PTPODINTST));

	switch (ptp_read_field(PERI_VCORE_PTPOD_CON0, VCORE_PTPODSEL)) {
	case SEL_VCORE_PDN:
		det = &ptp_detectors[PTP_DET_SOC];
		break;
	default:
		ptp_error("[%s]: Unknown det\n", __func__);
		break;
	}
#endif
	if (det) {
#if 0
		det->ops->switch_bank(det);
#else
		/*ptp_write_field(PERI_VCORE_PTPOD_CON0, VCORE_PTPODSEL, (det == &ptp_detectors[PTP_DET_VCORE_AO]) ? SEL_VCORE_AO : SEL_VCORE_PDN);
		ptp_write_field(PTP_PTPCORESEL, APBSEL, det->ctrl_id);*/
		ptp_write_field(PTP_PTPCORESEL, APBSEL, PTP_CTRL_SOC);
#endif
		// mt_ptp_reg_dump_locked(); // <-XXX
		ptp_isr_handler(det);
	}

	FUNC_EXIT(FUNC_LV_MODULE);
}

unsigned int vcore0 = 0;
unsigned int vcore1 = 0;
unsigned int vcore2 = 0;

void mt_set_ptp_info(ptp_info_t* ptp_info)
{
	ptp_info->first_volt= vcore0;
    ptp_info->second_volt= vcore1;    
    ptp_info->third_volt= vcore2;    
    ptp_info->have_550= have_550;    

    printf("[PTP][preloader]first_volt = 0x%x\n", ptp_info->first_volt);
    printf("[PTP][preloader]second_volt = 0x%x\n", ptp_info->second_volt);
    printf("[PTP][preloader]third_volt = 0x%x\n", ptp_info->third_volt);
    printf("[PTP][preloader]have_550 = 0x%x\n", ptp_info->have_550);
}
/*=============================================================
 * Global function definition
 *=============================================================*/

void ptp_init(void)
{
	FUNC_ENTER(FUNC_LV_MODULE);
	get_devinfo(&ptp_devinfo);
#if 0
	if (0 == ptp_devinfo.PTPINITEN) {
		ptp_notice("PTPINITEN = 0x%x\n", ptp_devinfo.PTPINITEN);

		return;
	}
#endif
	ptp_init_det(&ptp_detectors[PTP_DET_SOC], &ptp_devinfo);

	// TODO: FIXME, enable thermal CG - MT_CG_PERI_THERM
#if 0
	ptp_debug("TOP_DCMCTL = 0x%X\n", ptp_read(TOP_DCMCTL));

	base_ops_set_phase(&ptp_detectors[PTP_DET_SOC], PTP_PHASE_INIT01);
	ptp_isr();

	mt_ptp_reg_dump();

	base_ops_set_phase(&ptp_detectors[PTP_DET_SOC], PTP_PHASE_INIT02);
	ptp_isr();

	mt_ptp_reg_dump();
#endif
	{
		unsigned int vcore_volt;
#if 0
		ptp_detectors[PTP_DET_SOC].volt_tbl_init2[0] = (   0 == ptp_detectors[PTP_DET_SOC].MDES
								      && 0 == ptp_detectors[PTP_DET_SOC].BDES
								      && 0 == ptp_detectors[PTP_DET_SOC].DCMDET
								      && 0 == ptp_detectors[PTP_DET_SOC].DCBDET
								      ) ? 0 : ptp_detectors[PTP_DET_SOC].volt_tbl_init2[0];
#endif
		if (0x321 == mt_get_chip_hw_code()) {
			vcore_volt = ptp_detectors[PTP_DET_SOC].volt_tbl_bin[1];
			ptp_detectors[PTP_DET_SOC].volt_tbl_bin[0] = MV_TO_VAL(1250) + PTPOD_PMIC_OFFSET;
			vcore0 = ptp_detectors[PTP_DET_SOC].volt_tbl_bin[0];
			vcore1 = ptp_detectors[PTP_DET_SOC].volt_tbl_bin[1];
			vcore2 = ptp_detectors[PTP_DET_SOC].volt_tbl_bin[2];
		}
		else if (0x335 == mt_get_chip_hw_code()) {
			vcore_volt = ptp_detectors[PTP_DET_SOC].volt_tbl_bin[1];
		
			if (have_550 == 0)
				ptp_detectors[PTP_DET_SOC].volt_tbl_bin[0] = MV_TO_VAL(1150) + PTPOD_PMIC_OFFSET;

			vcore0 = ptp_detectors[PTP_DET_SOC].volt_tbl_bin[0];
			vcore1 = ptp_detectors[PTP_DET_SOC].volt_tbl_bin[1];
			vcore2 = ptp_detectors[PTP_DET_SOC].volt_tbl_bin[2];
			ptp_isr_info("vcore0 = 0x%x, vcore1 = 0x%x, vcore2 = 0x%x, have_550 = 0x%x\n", vcore0, vcore1, vcore2, have_550);
		}
		else if (0x337 == mt_get_chip_hw_code())
			vcore_volt = ptp_detectors[PTP_DET_SOC].volt_tbl_init2[0];
		else
			vcore_volt = MV_TO_VAL(1150) + PTPOD_PMIC_OFFSET;

		vcore_volt = (0 == vcore_volt) ? ptp_detectors[PTP_DET_SOC].VBOOT : vcore_volt;

		ptp_debug("PTP set volt: 0x%X\n", vcore_volt);

		if ( ptp_detectors[PTP_DET_SOC].VBOOT > vcore_volt) {
			int i;

			for (i = 1; (ptp_detectors[PTP_DET_SOC].VBOOT - i) >= vcore_volt; i++) {
				/* XXX: VCORE_AO >; VCORE_PDN */
				pmic_config_interface(0x618,(ptp_detectors[PTP_DET_SOC].VBOOT - i),0x7F,0);

				ptp_debug("0x%X\n", (ptp_detectors[PTP_DET_SOC].VBOOT - i));
			}
		}
	}

	{
		int *val = (int *)&ptp_devinfo;
		int i;

		for (i = 0; i < sizeof(struct ptp_devinfo)/sizeof(unsigned int); i++)
			ptp_debug("M_HW_RES%d\t= 0x%X\n", i, val[i]);
	}

	FUNC_EXIT(FUNC_LV_MODULE);
}
