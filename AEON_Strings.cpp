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

const char* AEON_Strings::monthOfYear[][2] = {
  /* Jan */ {"Jan", "Jan"},
  /* Feb */ {"Feb", "Feb"},
  /* Mar */ {"Mar", "Mär"},
  /* Apr */ {"Apr", "Apr"},
  /* May */ {"May", "Mai"},
  /* Jun */ {"Jun", "Jun"},
  /* Jul */ {"Jul", "Jul"},
  /* Aug */ {"Aug", "Aug"},
  /* Sep */ {"Sep", "Sep"},
  /* Oct */ {"Oct", "Okt"},
  /* Nov */ {"Nov", "Nov"},
  /* Dec */ {"Dec", "Dez"}
  };

const char* AEON_Strings::dayOfWeek[][2] = {
  /* Sun */ {"Sun", "So"},
  /* Mon */ {"Mon", "Mo"},
  /* Tue */ {"Tue", "Di"},
  /* Wed */ {"Wed", "Mi"},
  /* Thu */ {"Thu", "Do"},
  /* Fri */ {"Fri", "Fr"},
  /* Sat */ {"Sat", "Sa"}
};

// English, German
const char* AEON_Strings::text[][2] = {
  /*Error*/ {"ERROR", "Fehler"},
  /*OK*/ {"OK", "OK"},
  /*Remaining Days*/ {"Remaining Days", "Verbleibende Tage"},
  /*Setup*/ {"Setup", "Setup"},
  /*SetupTime*/ {"Setup Time", "Setup Zeit"},
  /*Time*/ {"Time", "Zeit"},
  /*SetupDate*/ {"Setup Date", "Setup Datum"},
  /*Date*/ {"Date", "Datum"},
  /*SetupBirthday*/ {"Setup Birthday", "Setup Geburtstag"},
  /*Birthday*/ {"Birthday", "Geburtstag"},
  /*SetupSex*/ {"Setup Sex", "Setup Geschlecht"},
  /*Sex*/ {"Sex", "Geschlecht"},
  /*Female*/ {"Female", "Frau"},
  /*Male*/ {"Male", "Mann"},
  /*SetupLifespan*/ {"Setup Lifespan", "Setup Lebenszeit"},
  /*Lifespan*/ {"Lifespan", "Lebenszeit"},
  /*SetupLanguage*/ {"Setup Language", "Setup Sprache"},
  /*Language*/ {"Language", "Sprache"},
  /*English*/ {"English", "Englisch"},
  /*German*/ {"German", "Deutsch"},
  /*SetupReset*/ {"Setup Reset", "Setup Zur\x81\x63ksetzen"},
  /*Reset*/ {"Reset", "Reset"},
  /*YES*/ {"YES", "JA"},
  /*NO*/ {"NO", "NEIN"},
  /*SetupBack*/ {"Back", "Zur\x81\x63\x6B"},
};