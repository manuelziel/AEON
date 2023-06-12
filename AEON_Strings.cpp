/*
AEON_Strings.cpp
*/

#include "AEON_Strings.h"
#include "AEON_Enums.h"
#include "AEON_ROM.h"

extern AEON_ROM rom;

/*
Get Text string
*/
const char *AEON_Strings::getString(EStrings string)
{
    ELanguage language = rom.getLanguage();

    return AEON_Strings::text[(int)string][language];
};

/*
Get weekday string
*/
const char *AEON_Strings::getWeekday(int weekday)
{
    ELanguage language = rom.getLanguage();

    return AEON_Strings::dayOfWeek[(int)weekday][language];
};

/*
Get month string
*/
const char *AEON_Strings::getMonth(int month)
{
    ELanguage language = rom.getLanguage();

    return AEON_Strings::monthOfYear[(int)month][language];
};

// Strings of the month of year English, German, French, Spain
const char* AEON_Strings::monthOfYear[][4] = {
  /* Jan */ {"Jan", "Jan", "Jan", "Ene"},
  /* Feb */ {"Feb", "Feb", "Fe`v", "Feb"},
  /* Mar */ {"Mar", "Mär", "Mar", "Mar"},
  /* Apr */ {"Apr", "Apr", "Avr", "Abr"},
  /* May */ {"May", "Mai", "Mai", "May"},
  /* Jun */ {"Jun", "Jun", "Juin", "Jun"},
  /* Jul */ {"Jul", "Jul", "Juil", "Jul"},
  /* Aug */ {"Aug", "Aug", "Aou^", "Ago"},
  /* Sep */ {"Sep", "Sep", "Sep", "Sep"},
  /* Oct */ {"Oct", "Okt", "Oct", "Oct"},
  /* Nov */ {"Nov", "Nov", "Nov", "Nov"},
  /* Dec */ {"Dec", "Dez", "De`c", "Dic"}
};

// Strings day of the week English, German, French, Spain
const char* AEON_Strings::dayOfWeek[][4] = {
  /* Sun */ {"Sun", "So", "Dim", "Dom"},
  /* Mon */ {"Mon", "Mo", "Lun", "Lun"},
  /* Tue */ {"Tue", "Di", "Mar", "Mar"},
  /* Wed */ {"Wed", "Mi", "Mer", "Mie`"},
  /* Thu */ {"Thu", "Do", "Jeu", "Jue"},
  /* Fri */ {"Fri", "Fr", "Ven", "Vie"},
  /* Sat */ {"Sat", "Sa", "Sam", "Sa`b"}
};

// Strings English, German, French, Spain
const char* AEON_Strings::text[][4] = {
  /*Error*/ {"Error", "Fehler", "Erreur", "Error"},
  /*OK*/ {"OK", "OK", "OK", "OK"},
  /*Remaining Days*/ {"Remaining Days", "Verbleibende Tage", "Jours restants", "Di`as restantes"},
  /*Setup*/ {"Setup", "Setup", "Config", "Config"},
  /*SetupTime*/ {"Setup Time", "Setup Zeit", "l'heure", "la hora"},
  /*Time*/ {"Time", "Zeit", "Heure", "Hora"},
  /*SetupDate*/ {"Setup Date", "Setup Datum", "la date", "la fecha"},
  /*Date*/ {"Date", "Datum", "Date", "Fecha"},
  /*SetupBirthday*/ {"Setup Birthday", "Setup Geburtstag", "l'anniversaire", "el cumpleanyos"},
  /*Birthday*/ {"Birthday", "Geburtstag", "Anniv.", "Cumple."},
  /*SetupSex*/ {"Setup Sex", "Setup Geschlecht", "le sexe", "el sexo"},
  /*Sex*/ {"Sex", "Geschlecht", "Sexe", "Sexo"},
  /*Female*/ {"Female", "Frau", "Femme", "Mujer"},
  /*Male*/ {"Male", "Mann", "Homme", "Hombre"},
  /*SetupLifespan*/ {"Setup Lifespan", "Setup Lebenszeit", "la dure`e de vie", "la esperanza de vida"},
  /*Lifespan*/ {"Lifespan", "Lebenszeit", "Dure`e.", "Esperanza."},
  /*SetupLanguage*/ {"Setup Language", "Setup Sprache", "a langue", "el idioma"},
  /*Language*/ {"Language", "Sprache", "Langue", "Idioma"},
  /*English*/ {"English", "Englisch", "Anglais", "Ingle`s"},
  /*German*/ {"German", "Deutsch", "Allemand", "Alema`n"},
  /*French*/ {"French", "Franz\x94sisch", "Fran\x87\x61is", "Franc\x82s"},
  /*Spain*/ {"Spain", "Spanisch", "Espagne", "Espanya"},
  /*SetupReset*/ {"Setup Reset", "Setup Zur\x81\x63ksetzen", "Re`initialiser", "Restablecer"},
  /*Reset*/ {"Reset", "Reset", "Re`initialiser", "Restablecer"},
  /*YES*/ {"YES", "JA", "OUI", "SI`"},
  /*NO*/ {"NO", "NEIN", "NON", "NO"},
  /*SetupBack*/ {"Back", "Zur\x81\x63\x6B", "Retour", "Volver"},
};