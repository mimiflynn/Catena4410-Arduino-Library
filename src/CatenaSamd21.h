/* CatenaSamd21.h	Wed Mar  8 2017 17:29:54 tmm */

/*

Module:  CatenaSamd21.h

Function:
	class CatenaSamd21

Version:
	V0.4.0	Wed Mar  8 2017 17:29:54 tmm	Edit level 3

Copyright notice:
	This file copyright (C) 2016-2017 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.

Author:
	Terry Moore, MCCI Corporation	October 2016

Revision history:
   0.1.0  Tue Oct 25 2016 02:00:28  tmm
	Module created.

   0.3.0  Thu Oct 27 2016 22:53:12  tmm
	Eliminate array types.

   0.4.0  Wed Mar  8 2017 17:29:54  tmm
	Add flags for new well-known sensors and mod-number provisions.

*/

#ifndef _CATENASAMD21_H_		/* prevent multiple includes */
#define _CATENASAMD21_H_

#ifndef _CATENABASE_H_
# include "CatenaBase.h"
#endif

#include "mcciadk_guid.h"

/* forward references */
struct CATENA_PLATFORM;
struct CATENA_CPUID_TO_PLATFORM;

/* the class for Samd21-based Catenas */
class CatenaSamd21 : public CatenaBase
	{
public:	
        using Super = CatenaBase;

	/*
	|| Types
	*/
	struct	UniqueID_buffer_t
		{
		uint8_t b[128/8];
		};

	/* a buffer big enough to hold a stringified UniqueID_buffer_t */
	struct UniqueID_string_t
		{
		char	c[sizeof(UniqueID_buffer_t) * 3 + 1];
		};

        // flags that describe generic platform capabilities
	enum PLATFORM_FLAGS : uint32_t
		{
                // platform has LoRa
		fHasLoRa = 1 << 0,
                // platform has Bluetooth LE
		fHasBLE = 1 << 1,
                // platform has Wi-Fi
		fHasWiFi = 1 << 2,
                // platform not only has LoRa, but it's wired according to TTN NYC standards
		fHasTtnNycLoRa = 1 << 3,
                // platform supports the BME280
		fHasBme280 = 1 << 4,
                // platform supports the Lux meter
		fHasLux = 1 << 5,
                // platform supports soil probe
		fHasSoilProbe = 1 << 6,
                // platform supports external solar panel
		fHasSolarPanel = 1 << 7,
                // platform supports one-wire temperature sensor
		fHasWaterOneWire = 1 << 8,
                // platform not only has LoRa, but it's wired per the MCCI RadioWing standard
                fHasTtnMcciLoRa = 1 << 9,
		// platform has the Rohm Lux meter
		fHasLuxRohm = 1 << 10,
		// platform has i2c mux
		fHasI2cMux = 1 << 11,
		// platfomr has 2kx8 FRAM
		fHasFRAM = 1 << 12,
		// special wiring variants all are offsets from M100...
		// we support up to 127 variants, becuase we have 7
		// bits and variant 0 means "base model".
		fModNumber = 0x7Fu << 25,
		// a few variant values that are well know.
		  fM101 = 0x01 << 25,
		  fM102 = 0x02 << 25,
		  fM103 = 0x03 << 25,
		};

	/*
	|| Methods
	*/

        // start the Samd21 level
	virtual bool begin(void);
	bool begin(uint32_t uOverrideMask);
	bool begin(uint32_t uClearMask, uint32_t uSetMask);

	// Get the model number from flags. constexpr to allow for
	// most aggressive optimization.
	static uint32_t constexpr PlatformFlags_GetModNumber(uint32_t flags)
		{
		return (flags & fModNumber) ? 100u + ((flags & fModNumber) / (fModNumber & (~fModNumber + 1u))) : 0;
		};

	// Return true if this unit has been modded. constexpr to allow for
	// most aggressive optimization.
	static bool constexpr PlatformFlags_IsModded(uint32_t flags)
		{
		return (flags & fModNumber) != 0;
		}

	const CATENA_PLATFORM *GetPlatformForID(
		const UniqueID_buffer_t *pIdBuffer,
		EUI64_buffer_t *pSysEUI
		);

	const CATENA_PLATFORM *GetPlatformForID(
		const UniqueID_buffer_t *pIdBuffer,
		EUI64_buffer_t *pSysEUI,
		uint32_t *pOperatingFlags
		);

	void GetUniqueID(
		UniqueID_buffer_t *pIdBuffer
		);

	char *GetUniqueIDstring(
		UniqueID_string_t *pIdStringBuffer
		);

	const inline CATENA_PLATFORM *GetPlatform(void)
		{
		return this->m_pPlatform;
		}
	inline uint32_t GetOperatingFlags(void)
		{
		return this->m_OperatingFlags;
		}
	inline uint32_t GetPlatformFlags(void);		

	/*
	|| Class-level information
	*/
private:
	static const CATENA_CPUID_TO_PLATFORM vCpuIdToPlatform[];
	static const size_t nvCpuIdToPlatform;
	uint32_t		m_OperatingFlags;
	const CATENA_PLATFORM	*m_pPlatform;
	};


/*

Name:	CatenaSamd21::PlatformFlags_GetModNumber()

Index:	Function:	CatenaSamd21::PlatformFlags_IsModded();

Function:
	Return M101 etc info about this Catena instance given platform flags.

Definition:
	#include <CatenaSamd21.h>

	static constexpr uint32_t
		CatenaSamd21::PlatformFlags_GetModNumber(
			uint32_t uPlatformFlags
			);


	static constexpr bool
		CatenaSamd21::PlatformFlags_IsModded(
			uint32_t uPlatformFlags
			);

Description:
	Catenas have a "stock" or "base" configuration -- this is how they
	are built by default. At MCCI, we track variants using "M numbers"
	(a concept that we got from the Ithaca electronics scene via Ithaco,
	and ultimately, no doubt, from GE). M numbers are simply unique
	3-digit numbers; normally they start with 101, and are assigned in
	sequence. For example, the Catena 4450-M101 has been optimized for
	AC power measurement use.

	We reserve 7 bits in the platform flags for representing M-numbers.
	Initially, at any rate, your code must know what the numbers mean.

Returns:
	CatenaSamd21::PlatformFlags_GetModNumber() extracts the mod-number
	from the platform flags, and returns it as a number. If there is no
	mod number for this device, then this will return zero; otherwise it
	returns the mod number (which is always in the range [101..227].

	CatenaSamd21::PlatformFlags_IsModded() returns true if the platform
	flags indicate that this instance has a non-zero mod number.

*/
// actual function is above.

/*

Type:	CATENA_PLATFORM

Function:
	Represents common info about any Catena variant.

Description:
	Every Catena model is represented by a CATENA_PLATFORM instance.
	This instance respresents common information about all Catenas of
	that kind.

	The platforms are organized as a tree; each node has a pointer to
	a parent node which is a more general version of the same platform.

Contents:
	MCCIADK_GUID Guid;
		The GUID fo rthis platform.

	const CATENA_PLATFORM *pParent;
		The parent platform, or NULL if this is the root for
		this family of models.

	uint32_t PlatformFlags;
		The flags describing the capabilites of this platform. These
		are formed by oring together flags from
		CatenaSam21::PLATFORM_FLAGS.

	uint32_t OperatingFlags;
		Default operating flags. The actual operating flags may be
		modified on a per-device basis.

Notes:
	Typically the platforms are referenced by name from the table of well-
	known CPU IDs, or from the code that supplies the default platform.

See Also:
	CatenaSamd21, CATENA_CPUID_TO_PLATFORM

*/

struct CATENA_PLATFORM
	{
	MCCIADK_GUID		Guid;
	const CATENA_PLATFORM	*pParent;
	uint32_t		PlatformFlags;
	uint32_t		OperatingFlags;
	};

inline uint32_t CatenaSamd21::GetPlatformFlags(void)
	{
	const CATENA_PLATFORM * const pPlatform = this->m_pPlatform;

	if (pPlatform != NULL)
		return pPlatform->PlatformFlags;
	else
		return 0;
	}

/*

Type:	CATENA_CPUID_TO_PLATFORM

Function:
	Simple structure to map a CPU ID to a Platform code.

Description:
	Many Catenas don't have NVRAM, and need to have information about
	how the SAMD21 CPU is wired up and connected to the world. However,
	all SAMD21s have a unique CPU ID. We take advantage of this, and the
	relatively large Flash memory of the SAMD21, to map the unique CPU ID
	onto a platform pointer. Unfortunately this requires manually adding
	CPU IDs to the flash table, so this is only suitable for small volume;
	but it works well if the number of CPUs is less than a hundred or so.

	The file gk_WellKnownCpuBindings.cpp instantiates an array,
	gk_WellKnownCpuBindings[], containing CPU ID => platform mappings.

	For convenience, we also allow you to override operating flags on
	a CPU-ID-by-CPU-ID basis.

	Catenas with NVRAM use a similar concept, but the data in the NVRAM
	provides a platform GUID, and the SysEUI, rather than mapping the
	CPUID to a platform pointer. The platform GUID is used to find
	a CATENA_PLATFORM, and the SysEUI provides the identity.

Contents:
	CatenaSamd21::UniqueID_buffer_t	CpuID;
		The Unique CPU ID.

	const CATENA_PLATFORM *pPlatform;
		A pointer to the platform that defines this assembly.

	CatenaBase::EUI64_buffer_t	SysEUI;
		The EUI64 for this platform. If zero, then none is defined.

	uint32_t OperatingFlagsClear;
	uint32_t OperatingFlagsSet;
		These two entries, taken together, are used to selectively
		clear and set bits in the operating flags. Bits set in
		OperatingFlagsClear will be cleared in the operating flags.
		Bits set in OperatingFlagsSet will be set in the operating
		flags. Clear is applied before set.

Notes:
	The following bash macro is generally used to generate the first
	two lines of initiailization given the output from an MCCI test
	program:

.	function _cpuid {
.	        echo "$1" |
.	        sed -e 's/^/0x/' -e 's/-/, 0x/g' |
.		awk '{
.	                $1 = "        { CpuID:  { " $1;
.		        $8 = $8 "\n\t\t   ";
.			$16 = $16 " },";
.	                print }' ;
.	}

	With this macro in scope, you can write (e.g.):

.	_cpuid 05-25-dc-ef-54-53-4b-50-4a-31-2e-39-36-1a-07-ff

	and you'll get the first two lines of a suitable table entry.

See Also:
	gk_WellKnownCpuBindings[], CATENA_PLATFORM

*/

struct CATENA_CPUID_TO_PLATFORM
	{
	CatenaSamd21::UniqueID_buffer_t	CpuID;

	const CATENA_PLATFORM		*pPlatform;
	CatenaBase::EUI64_buffer_t	SysEUI;
	uint32_t			OperatingFlagsClear;
	uint32_t			OperatingFlagsSet;
	};

/**** end of CatenaSamd21.h ****/
#endif /* _CATENASAMD21_H_ */
