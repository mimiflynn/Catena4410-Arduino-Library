/* catenafeatherm0_lorawan_getprovisioningstyle.cpp	Mon Oct 31 2016 17:06:29 tmm */

/*

Module:  catenafeatherm0_lorawan_getprovisioningstyle.cpp

Function:
	CatenaFeatherM0::LoRaWAN::GetProvisioningStyle().

Version:
	V0.3.0	Mon Oct 31 2016 17:06:29 tmm	Edit level 1

Copyright notice:
	This file copyright (C) 2016 by

		MCCI Corporation
		3520 Krums Corners Road
                Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	October 2016

Revision history:
   0.3.0  Mon Oct 31 2016 17:06:29  tmm
	Module created. Barbarously long name needed to prevent
        in-library collisions.

*/

#include <CatenaFeatherM0.h>

#include <Arduino_LoRaWAN_lmic.h>

/****************************************************************************\
|
|		Manifest constants & typedefs.
|
\****************************************************************************/



/****************************************************************************\
|
|	Read-only data.
|
\****************************************************************************/



/****************************************************************************\
|
|	VARIABLES:
|
\****************************************************************************/


Arduino_LoRaWAN::ProvisioningStyle
CatenaFeatherM0::LoRaWAN::GetProvisioningStyle(
        void
        )
        {
        CatenaFeatherM0 * const pCatena = this->m_pCatena;
        const ProvisioningInfo * const pInstance = pCatena->GetProvisioningInfo();

        if (! pInstance)
                {
                ARDUINO_LORAWAN_PRINTF(
                        LogVerbose,
                        "%s: no provisioning info\n",
                        __func__
                        );

                return Arduino_LoRaWAN::ProvisioningStyle::kNone;
                }
        else
                {
                return pInstance->Style;
                }
        }
