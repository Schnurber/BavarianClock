/* Bavarian Language */

const char* bav_minutes[] = {
  "Akurrat",
  "Fünf noch",
  "Zehn noch",
  "Vieatl",
  "Zehn voa hoiber",
  "Fünf voa hoiber",
  "Akurrat hoiber",
  "Fünf noch hoiber",
  "Fünf voa dreivieatl",
  "Dreiviatl",
  "Zehn voa",
  "Fünf voa"
};

const char* bav_hours[] = {
  "zwölfe af d´Nocht", //0
  "oans in da Fria",
  "zwoa in da Fria",
  "dreie in da Fria",
  "viere in da Fria",
  "fünfe in da Fria",
  "sechse in da Fria",
  "sieme in da Fria",
  "achte in da Fria",
  "neine Voamidoch",
  "zehne Voamidoch",
  "eife Voamidoch",
  "zwölfe Midoch",
  "oans Nomidoch",
  "zwoa Nomidoch",
  "dreie Nomidoch",
  "viere Nomidoch",
  "fünfe oumds",
  "sechse oumds",
  "sieme oumds",
  "achte oumds",
  "neine af d´Nocht",
  "zehne af d´Nocht",
  "eife af d´Nocht"
};
const char* bav_days[] = {
  "Sunda",
  "Mouda",
  "Iada",
  "Mitta",
  "Pfinzda",
  "Freita",
  "Samsda"
};
static int getHours(int minutes, int hours) {

  if (minutes>=15) {
    hours ++; //Ab Viertel
    if (hours >23) { // Dann wieder von vorn
      hours =0;
    }
  }
  return hours;
}
