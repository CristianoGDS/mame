#include <assert.h>
#include "chdcd.h"
#include "harddisk.h"
#include <windows.h>
#define OSD_WINDOWS
#include "strconv.h"

#define BUILD_LIB
#include "libchd.h"

int libchd_cdrom_open(const WCHAR *input_name)
{
	cdrom_file *cdrom = NULL;
	chd_file *input_chd = new chd_file;

	if (input_chd == NULL) return 0;

	chd_error err = input_chd->open(utf8_from_wstring(input_name), false, NULL);

	if (err == CHDERR_NONE)
		cdrom = cdrom_open(input_chd);
	else
		cdrom = cdrom_open(utf8_from_wstring(input_name));

	if (cdrom == NULL)
	{
		if (err == CHDERR_NONE)
			input_chd->close();
		delete input_chd;
		return 0;
	}

	if (err != CHDERR_NONE)
		delete input_chd;

	return (int)cdrom;
}

void libchd_cdrom_close(cdrom_file *cdrom)
{
	if (cdrom != NULL)
	{
		chd_file *input_chd = cdrom_get_chd(cdrom);
		if (input_chd != NULL)
		{
			input_chd->close();
			delete input_chd;
		}
		cdrom_close(cdrom);
		cdrom = NULL;
	}
}

int libchd_cdrom_get_toc(cdrom_file *cdrom, const cdrom_toc **toc)
{
	if (cdrom == NULL) return 0;

	*toc = cdrom_get_toc(cdrom);
	return 1;
}

int libchd_cdrom_get_version(cdrom_file *cdrom)
{
	if (cdrom != NULL)
	{
		chd_file *input_chd = cdrom_get_chd(cdrom);
		if (input_chd != NULL)
			return input_chd->version();
	}
	return -1;
}

int libchd_cdrom_read_data(cdrom_file *cdrom, UINT32 lbasector, void *buffer, UINT32 datatype, bool phys)
{
	if (cdrom == NULL) return 0;

	return cdrom_read_data(cdrom, lbasector, buffer, datatype, phys);
}

int libchd_cdrom_read_subcode(cdrom_file *cdrom, UINT32 lbasector, void *buffer, bool phys)
{
	if (cdrom == NULL) return 0;

	return cdrom_read_subcode(cdrom, lbasector, buffer, phys);
}



int libchd_hard_disk_open(const WCHAR *input_name, bool mode)
{
	hard_disk_file *hdd = NULL;
	chd_file *input_chd = new chd_file;

	if (input_chd == NULL) return 0;

	chd_error err = input_chd->open(utf8_from_wstring(input_name), mode, NULL);

	if (err != CHDERR_NONE)
	{
		delete input_chd;
		return 0;
	}

	hdd = hard_disk_open(input_chd);

	if (hdd == NULL)
	{
		input_chd->close();
		delete input_chd;
		return 0;
	}

	return (int)hdd;
}

int libchd_hard_disk_get_info(hard_disk_file *hdd)
{
	if (hdd == NULL) return 0;

	return (int)hard_disk_get_info(hdd);
}

void libchd_hard_disk_close(hard_disk_file *hdd)
{
	if (hdd != NULL)
	{
		chd_file *input_chd = hard_disk_get_chd(hdd);
		input_chd->close();
		delete input_chd;
		hard_disk_close(hdd);
		hdd = NULL;
	}
}

int libchd_hard_disk_read(hard_disk_file *hdd, UINT32 lbasector, void *buffer)
{
	if (hdd == NULL) return 0;

	return hard_disk_read(hdd, lbasector, buffer);
}

int libchd_hard_disk_write(hard_disk_file *hdd, UINT32 lbasector, void *buffer)
{
	if (hdd == NULL) return 0;

	return hard_disk_write(hdd, lbasector, buffer);
}
