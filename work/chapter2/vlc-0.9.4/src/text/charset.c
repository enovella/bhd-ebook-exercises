/*****************************************************************************
 * charset.c: Locale's character encoding stuff.
 *****************************************************************************
 * See also unicode.c for Unicode to locale conversion helpers.
 *
 * Copyright (C) 2003-2008 the VideoLAN team
 *
 * Authors: Christophe Massiot
 *          Rémi Denis-Courmont
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vlc_common.h>

#if !defined WIN32
# include <locale.h>
#else
# include <windows.h>
#endif

#ifdef __APPLE__
#   include <errno.h>
#   include <string.h>
#   include <xlocale.h>
#endif

#include "libvlc.h"
#include <vlc_charset.h>

char *vlc_fix_readdir( const char *psz_string )
{
#ifdef __APPLE__
    vlc_iconv_t hd = vlc_iconv_open( "UTF-8", "UTF-8-MAC" );

    if (hd != (vlc_iconv_t)(-1))
    {
        const char *psz_in = psz_string;
        size_t i_in = strlen(psz_in);
        size_t i_out = i_in * 2;
        char *psz_utf8 = malloc(i_out + 1);
        char *psz_out = psz_utf8;

        size_t i_ret = vlc_iconv (hd, &psz_in, &i_in, &psz_out, &i_out);
        vlc_iconv_close (hd);
        if( i_ret == (size_t)(-1) || i_in )
        {
            free( psz_utf8 );
            return strdup( psz_string );
        }

        *psz_out = '\0';
        return psz_utf8;
    }
#endif
    return strdup( psz_string );
}


/**
 * us_strtod() has the same prototype as ANSI C strtod() but it uses the
 * POSIX/C decimal format, regardless of the current numeric locale.
 */
double us_strtod( const char *str, char **end )
{
    locale_t loc = newlocale (LC_NUMERIC_MASK, "C", NULL);
    locale_t oldloc = uselocale (loc);
    double res = strtod (str, end);

    if (loc != (locale_t)0)
    {
        uselocale (oldloc);
        freelocale (loc);
    }
    return res;
}

/**
 * us_atof() has the same prototype as ANSI C atof() but it expects a dot
 * as decimal separator, regardless of the system locale.
 */
double us_atof( const char *str )
{
    return us_strtod( str, NULL );
}

