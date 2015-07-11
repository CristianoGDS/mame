#ifndef __LIBCHD_H__
#define __LIBCHD_H__


#ifndef BUILD_LIB
/***************************************************************************
CONSTANTS
***************************************************************************/

// tracks are padded to a multiple of this many frames
const UINT32 CD_TRACK_PADDING = 4;

#define CD_MAX_TRACKS           (99)    /* AFAIK the theoretical limit */
#define CD_MAX_SECTOR_DATA      (2352)
#define CD_MAX_SUBCODE_DATA     (96)

#define CD_FRAME_SIZE           (CD_MAX_SECTOR_DATA + CD_MAX_SUBCODE_DATA)
#define CD_FRAMES_PER_HUNK      (8)

#define CD_METADATA_WORDS       (1+(CD_MAX_TRACKS * 6))

enum
{
	CD_TRACK_MODE1 = 0,         /* mode 1 2048 bytes/sector */
	CD_TRACK_MODE1_RAW,         /* mode 1 2352 bytes/sector */
	CD_TRACK_MODE2,             /* mode 2 2336 bytes/sector */
	CD_TRACK_MODE2_FORM1,       /* mode 2 2048 bytes/sector */
	CD_TRACK_MODE2_FORM2,       /* mode 2 2324 bytes/sector */
	CD_TRACK_MODE2_FORM_MIX,    /* mode 2 2336 bytes/sector */
	CD_TRACK_MODE2_RAW,         /* mode 2 2352 bytes / sector */
	CD_TRACK_AUDIO,			    /* redbook audio track 2352 bytes/sector (588 samples) */

	CD_TRACK_RAW_DONTCARE       /* special flag for cdrom_read_data: just return me whatever is there */
};

enum
{
	CD_SUB_NORMAL = 0,          /* "cooked" 96 bytes per sector */
	CD_SUB_RAW,                 /* raw uninterleaved 96 bytes per sector */
	CD_SUB_NONE                 /* no subcode data stored */
};

#define CD_FLAG_GDROM   0x00000001  // disc is a GD-ROM, all tracks should be stored with GD-ROM metadata
#define CD_FLAG_GDROMLE 0x00000002  // legacy GD-ROM, with little-endian CDDA data

/***************************************************************************
TYPE DEFINITIONS
***************************************************************************/

struct cdrom_track_info
{
	/* fields used by CHDMAN and in MAME */
	UINT32 trktype;     /* track type */
	UINT32 subtype;     /* subcode data type */
	UINT32 datasize;    /* size of data in each sector of this track */
	UINT32 subsize;     /* size of subchannel data in each sector of this track */
	UINT32 frames;      /* number of frames in this track */
	UINT32 extraframes; /* number of "spillage" frames in this track */
	UINT32 pregap;      /* number of pregap frames */
	UINT32 postgap;     /* number of postgap frames */
	UINT32 pgtype;      /* type of sectors in pregap */
	UINT32 pgsub;       /* type of subchannel data in pregap */
	UINT32 pgdatasize;  /* size of data in each sector of the pregap */
	UINT32 pgsubsize;   /* size of subchannel data in each sector of the pregap */

	/* fields used in CHDMAN only */
	UINT32 padframes;   /* number of frames of padding to add to the end of the track; needed for GDI */

	/* fields used in MAME/MESS only */
	UINT32 logframeofs;	/* logical frame of actual track data - offset by pregap size if pregap not physically present */
	UINT32 physframeofs;/* physical frame of actual track data in CHD data */
	UINT32 chdframeofs; /* frame number this track starts at on the CHD */
};

struct cdrom_toc
{
	UINT32 numtrks;     /* number of tracks */
	UINT32 flags;       /* see FLAG_ above */
	cdrom_track_info tracks[CD_MAX_TRACKS];
};

struct hard_disk_info
{
	UINT32          cylinders;
	UINT32          heads;
	UINT32          sectors;
	UINT32          sectorbytes;
};
#endif


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef BUILD_LIB
#define LIBSPEC __declspec(dllexport)
#else
#define LIBSPEC __declspec(dllimport)
struct cdrom_file;
struct hard_disk_file;
#endif

int  LIBSPEC libchd_cdrom_open(const WCHAR *input_name);
void LIBSPEC libchd_cdrom_close(cdrom_file *cdrom);
int  LIBSPEC libchd_cdrom_get_toc(cdrom_file *cdrom, const cdrom_toc **toc);
int  LIBSPEC libchd_cdrom_get_version(cdrom_file *cdrom);
int  LIBSPEC libchd_cdrom_read_data(cdrom_file *cdrom, UINT32 lbasector, void *buffer, UINT32 datatype, bool phys);
int  LIBSPEC libchd_cdrom_read_subcode(cdrom_file *cdrom, UINT32 lbasector, void *buffer, bool phys);
int  LIBSPEC libchd_hard_disk_open(const WCHAR *input_name, bool mode);
void LIBSPEC libchd_hard_disk_close(hard_disk_file *hdd);
int  LIBSPEC libchd_hard_disk_get_info(hard_disk_file *hdd);
int  LIBSPEC libchd_hard_disk_read(hard_disk_file *hdd, UINT32 lbasector, void *buffer);
int  LIBSPEC libchd_hard_disk_write(hard_disk_file *hdd, UINT32 lbasector, void *buffer);

#ifdef __cplusplus
}
#endif

#endif /* __LIBCHD_H__ */
