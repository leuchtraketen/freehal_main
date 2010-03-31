/*
 * This file is part of FreeHAL 2010.
 *
 * Copyright(c) 2006, 2007, 2008, 2009, 2010 Tobias Schulz and contributors.
 * http://freehal.org
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include "parrot/parrot.h"

/* proto is in embed.h, but we don't include anything here, which
 * could pull in some globals
 */
void Parrot_set_config_hash(void);

void
Parrot_set_config_hash_internal (const unsigned char* parrot_config,
                                 unsigned int parrot_config_size);


static const unsigned char parrot_config[] = {
    0
}; /* parrot_config */

void
Parrot_set_config_hash(void)
{
    Parrot_set_config_hash_internal(parrot_config, sizeof(parrot_config));
}

/*
 * Local variables:
 *   c-file-style: "parrot"
 *   buffer-read-only: t
 * End:
 * vim: expandtab shiftwidth=4:
 */
