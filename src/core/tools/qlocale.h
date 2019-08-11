/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2019 Ivailo Monev
**
** This file is part of the QtCore module of the Katie Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QLOCALE_H
#define QLOCALE_H

#include <QtCore/qvariant.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE


class QDataStream;
class QDate;
class QDateTime;
class QTime;
class QVariant;
class QTextStream;
class QTextStreamPrivate;

class QLocale;

#ifndef QT_NO_SYSTEMLOCALE
class Q_CORE_EXPORT QSystemLocale
{
public:
    QSystemLocale();
    virtual ~QSystemLocale();

    struct CurrencyToStringArgument
    {
        CurrencyToStringArgument() { }
        CurrencyToStringArgument(const QVariant &v, const QString &s)
            : value(v), symbol(s) { }
        QVariant value;
        QString symbol;
    };

    enum QueryType {
        LanguageId, // uint
        CountryId, // uint
        DecimalPoint, // QString
        GroupSeparator, // QString
        ZeroDigit, // QString
        NegativeSign, // QString
        DateFormatLong, // QString
        DateFormatShort, // QString
        TimeFormatLong, // QString
        TimeFormatShort, // QString
        DayNameLong, // QString, in: int
        DayNameShort, // QString, in: int
        MonthNameLong, // QString, in: int
        MonthNameShort, // QString, in: int
        DateToStringLong, // QString, in: QDate
        DateToStringShort, // QString in: QDate
        TimeToStringLong, // QString in: QTime
        TimeToStringShort, // QString in: QTime
        DateTimeFormatLong, // QString
        DateTimeFormatShort, // QString
        DateTimeToStringLong, // QString in: QDateTime
        DateTimeToStringShort, // QString in: QDateTime
        MeasurementSystem, // uint
        PositiveSign, // QString
        AMText, // QString
        PMText, // QString
        FirstDayOfWeek, // Qt::DayOfWeek
        Weekdays, // QList<Qt::DayOfWeek>
        CurrencySymbol, // QString in: CurrencyToStringArgument
        CurrencyToString, // QString in: qlonglong, qulonglong or double
        UILanguages, // QStringList
        StringToStandardQuotation, // QString in: QStringRef to quote
        StringToAlternateQuotation, // QString in: QStringRef to quote
        ScriptId, // uint
        ListToSeparatedString, // QString
        LocaleChanged, // system locale changed
        NativeLanguageName, // QString
        NativeCountryName, // QString
        StandaloneMonthNameLong, // QString, in: int
        StandaloneMonthNameShort // QString, in: int
    };
    virtual QVariant query(QueryType type, QVariant in) const;
    virtual QLocale fallbackLocale() const;

private:
    QSystemLocale(bool);
    friend class QSystemLocaleSingleton;
};
#endif // QT_NO_SYSTEMLOCALE

struct QLocalePrivate;
class Q_CORE_EXPORT QLocale
{
    Q_GADGET
    Q_ENUMS(Language)
    Q_ENUMS(Country)
    friend class QString;
    friend class QByteArray;
    friend class QIntValidator;
    friend class QDoubleValidatorPrivate;
    friend class QTextStream;
    friend class QTextStreamPrivate;

public:
// GENERATED PART STARTS HERE
// see qlocale_data_p.h for more info on generated data
    enum Language {
        AnyLanguage = 0,
        C = 1,
        Abkhazian = 2,
        Achinese = 3,
        Acoli = 4,
        Adangme = 5,
        Adyghe = 6,
        Afar = 7,
        Afrihili = 8,
        Afrikaans = 9,
        Aghem = 10,
        Ainu = 11,
        Akan = 12,
        Akkadian = 13,
        Akoose = 14,
        Alabama = 15,
        Albanian = 16,
        Aleut = 17,
        AlgerianArabic = 18,
        AmericanEnglish = 19,
        AmericanSignLanguage = 20,
        Amharic = 21,
        AncientEgyptian = 22,
        AncientGreek = 23,
        Angika = 24,
        AoNaga = 25,
        Arabic = 26,
        Aragonese = 27,
        Aramaic = 28,
        Araona = 29,
        Arapaho = 30,
        Arawak = 31,
        Armenian = 32,
        Aromanian = 33,
        Arpitan = 34,
        Assamese = 35,
        Asturian = 36,
        Asu = 37,
        Atsam = 38,
        AustralianEnglish = 39,
        AustrianGerman = 40,
        Avaric = 41,
        Avestan = 42,
        Awadhi = 43,
        Aymara = 44,
        Azerbaijani = 45,
        Badaga = 46,
        Bafia = 47,
        Bafut = 48,
        Bakhtiari = 49,
        Balinese = 50,
        Baluchi = 51,
        Bambara = 52,
        Bamun = 53,
        Bangla = 54,
        Banjar = 55,
        Basaa = 56,
        Bashkir = 57,
        Basque = 58,
        BatakToba = 59,
        Bavarian = 60,
        Beja = 61,
        Belarusian = 62,
        Bemba = 63,
        Bena = 64,
        Betawi = 65,
        Bhojpuri = 66,
        Bikol = 67,
        Bini = 68,
        Bishnupriya = 69,
        Bislama = 70,
        Blin = 71,
        Blissymbols = 72,
        Bodo = 73,
        Bosnian = 74,
        Brahui = 75,
        Braj = 76,
        BrazilianPortuguese = 77,
        Breton = 78,
        BritishEnglish = 79,
        Buginese = 80,
        Bulgarian = 81,
        Bulu = 82,
        Buriat = 83,
        Burmese = 84,
        Caddo = 85,
        CajunFrench = 86,
        CanadianEnglish = 87,
        CanadianFrench = 88,
        Cantonese = 89,
        Capiznon = 90,
        Carib = 91,
        Catalan = 92,
        Cayuga = 93,
        Cebuano = 94,
        CentralAtlasTamazight = 95,
        CentralDusun = 96,
        CentralKurdish = 97,
        CentralYupik = 98,
        ChadianArabic = 99,
        Chagatai = 100,
        Chakma = 101,
        Chamorro = 102,
        Chechen = 103,
        Cherokee = 104,
        Cheyenne = 105,
        Chibcha = 106,
        Chiga = 107,
        ChimborazoHighlandQuichua = 108,
        Chinese = 109,
        ChinookJargon = 110,
        Chipewyan = 111,
        Choctaw = 112,
        ChurchSlavic = 113,
        Chuukese = 114,
        Chuvash = 115,
        ClassicalNewari = 116,
        ClassicalSyriac = 117,
        Colognian = 118,
        Comorian = 119,
        CongoSwahili = 120,
        Coptic = 121,
        Cornish = 122,
        Corsican = 123,
        Cree = 124,
        Creek = 125,
        CrimeanTurkish = 126,
        Croatian = 127,
        Czech = 128,
        Dakota = 129,
        Danish = 130,
        Dargwa = 131,
        Dari = 132,
        Dazaga = 133,
        Delaware = 134,
        Dinka = 135,
        Divehi = 136,
        Dogri = 137,
        Dogrib = 138,
        Duala = 139,
        Dutch = 140,
        Dyula = 141,
        Dzongkha = 142,
        EasternFrisian = 143,
        Efik = 144,
        EgyptianArabic = 145,
        Ekajuk = 146,
        Elamite = 147,
        Embu = 148,
        Emilian = 149,
        English = 150,
        Erzya = 151,
        Esperanto = 152,
        Estonian = 153,
        EuropeanPortuguese = 154,
        EuropeanSpanish = 155,
        Ewe = 156,
        Ewondo = 157,
        Extremaduran = 158,
        Fang = 159,
        Fanti = 160,
        Faroese = 161,
        FijiHindi = 162,
        Fijian = 163,
        Filipino = 164,
        Finnish = 165,
        Flemish = 166,
        Fon = 167,
        Frafra = 168,
        French = 169,
        Friulian = 170,
        Fulah = 171,
        Ga = 172,
        Gagauz = 173,
        Galician = 174,
        GanChinese = 175,
        Ganda = 176,
        Gayo = 177,
        Gbaya = 178,
        Geez = 179,
        Georgian = 180,
        German = 181,
        GhegAlbanian = 182,
        Ghomala = 183,
        Gilaki = 184,
        Gilbertese = 185,
        GoanKonkani = 186,
        Gondi = 187,
        Gorontalo = 188,
        Gothic = 189,
        Grebo = 190,
        Greek = 191,
        Guarani = 192,
        Gujarati = 193,
        Gusii = 194,
        Gwichin = 195,
        Haida = 196,
        HaitianCreole = 197,
        HakkaChinese = 198,
        Hausa = 199,
        Hawaiian = 200,
        Hebrew = 201,
        Herero = 202,
        Hiligaynon = 203,
        Hindi = 204,
        HiriMotu = 205,
        Hittite = 206,
        Hmong = 207,
        Hungarian = 208,
        Hupa = 209,
        Iban = 210,
        Ibibio = 211,
        Icelandic = 212,
        Ido = 213,
        Igbo = 214,
        Iloko = 215,
        InariSami = 216,
        Indonesian = 217,
        Ingrian = 218,
        Ingush = 219,
        Interlingua = 220,
        Interlingue = 221,
        Inuktitut = 222,
        Inupiaq = 223,
        Irish = 224,
        Italian = 225,
        JamaicanCreoleEnglish = 226,
        Japanese = 227,
        Javanese = 228,
        Jju = 229,
        JolaFonyi = 230,
        JudeoArabic = 231,
        JudeoPersian = 232,
        Jutish = 233,
        Kabardian = 234,
        Kabuverdianu = 235,
        Kabyle = 236,
        Kachin = 237,
        Kaingang = 238,
        Kako = 239,
        Kalaallisut = 240,
        Kalenjin = 241,
        Kalmyk = 242,
        Kamba = 243,
        Kanembu = 244,
        Kannada = 245,
        Kanuri = 246,
        KaraKalpak = 247,
        KarachayBalkar = 248,
        Karelian = 249,
        Kashmiri = 250,
        Kashubian = 251,
        Kawi = 252,
        Kazakh = 253,
        Kenyang = 254,
        Khasi = 255,
        Khmer = 256,
        Khotanese = 257,
        Khowar = 258,
        Kiche = 259,
        Kikuyu = 260,
        Kimbundu = 261,
        Kinaraya = 262,
        Kinyarwanda = 263,
        Kirghiz = 264,
        Kirmanjki = 265,
        Klingon = 266,
        Kom = 267,
        Komi = 268,
        KomiPermyak = 269,
        Kongo = 270,
        Konkani = 271,
        Korean = 272,
        Koro = 273,
        Kosraean = 274,
        Kotava = 275,
        KoyraChiini = 276,
        KoyraboroSenni = 277,
        Kpelle = 278,
        Krio = 279,
        Kuanyama = 280,
        Kumyk = 281,
        Kurdish = 282,
        Kurukh = 283,
        Kutenai = 284,
        Kwasio = 285,
        Ladino = 286,
        Lahnda = 287,
        Lakota = 288,
        Lamba = 289,
        Langi = 290,
        Lao = 291,
        Latgalian = 292,
        Latin = 293,
        LatinAmericanSpanish = 294,
        Latvian = 295,
        Laz = 296,
        Lezghian = 297,
        Ligurian = 298,
        Limburgish = 299,
        Lingala = 300,
        LinguaFrancaNova = 301,
        LiteraryChinese = 302,
        Lithuanian = 303,
        Livonian = 304,
        Lojban = 305,
        Lombard = 306,
        LouisianaCreole = 307,
        LowGerman = 308,
        LowSaxon = 309,
        LowerSilesian = 310,
        LowerSorbian = 311,
        Lozi = 312,
        LubaKatanga = 313,
        LubaLulua = 314,
        Luiseno = 315,
        LuleSami = 316,
        Lunda = 317,
        Luo = 318,
        Luxembourgish = 319,
        Luyia = 320,
        Maba = 321,
        Macedonian = 322,
        Machame = 323,
        Madurese = 324,
        Mafa = 325,
        Magahi = 326,
        MainFranconian = 327,
        Maithili = 328,
        Makasar = 329,
        MakhuwaMeetto = 330,
        Makonde = 331,
        Malagasy = 332,
        Malay = 333,
        Malayalam = 334,
        Maltese = 335,
        Manchu = 336,
        Mandar = 337,
        Mandingo = 338,
        Manipuri = 339,
        Manx = 340,
        Maori = 341,
        Mapuche = 342,
        Marathi = 343,
        Mari = 344,
        Marshallese = 345,
        Marwari = 346,
        Masai = 347,
        Mazanderani = 348,
        Medumba = 349,
        Mende = 350,
        Mentawai = 351,
        Meru = 352,
        Meta = 353,
        MexicanSpanish = 354,
        MiddleDutch = 355,
        MiddleEnglish = 356,
        MiddleFrench = 357,
        MiddleHighGerman = 358,
        MiddleIrish = 359,
        Mikmaq = 360,
        MinNanChinese = 361,
        Minangkabau = 362,
        Mingrelian = 363,
        Mirandese = 364,
        Mizo = 365,
        ModernStandardArabic = 366,
        Mohawk = 367,
        Moksha = 368,
        Moldavian = 369,
        Mongo = 370,
        Mongolian = 371,
        Montenegrin = 372,
        Morisyen = 373,
        MoroccanArabic = 374,
        Mossi = 375,
        Multiplelanguages = 376,
        Mundang = 377,
        MuslimTat = 378,
        Myene = 379,
        NKo = 380,
        NajdiArabic = 381,
        Nama = 382,
        NauruLanguage = 383,
        Navajo = 384,
        Ndonga = 385,
        Neapolitan = 386,
        Nepali = 387,
        Newari = 388,
        Ngambay = 389,
        Ngiemboon = 390,
        Ngomba = 391,
        Nheengatu = 392,
        Nias = 393,
        NigerianPidgin = 394,
        Niuean = 395,
        Nogai = 396,
        Nolinguisticcontent = 397,
        NorthNdebele = 398,
        NorthernFrisian = 399,
        NorthernLuri = 400,
        NorthernSami = 401,
        NorthernSotho = 402,
        Norwegian = 403,
        NorwegianBokmal = 404,
        NorwegianNynorsk = 405,
        Novial = 406,
        Nuer = 407,
        Nyamwezi = 408,
        Nyanja = 409,
        Nyankole = 410,
        NyasaTonga = 411,
        Nyoro = 412,
        Nzima = 413,
        Occitan = 414,
        Odia = 415,
        Ojibwa = 416,
        OldEnglish = 417,
        OldFrench = 418,
        OldHighGerman = 419,
        OldIrish = 420,
        OldNorse = 421,
        OldPersian = 422,
        OldProvencal = 423,
        Oromo = 424,
        Osage = 425,
        Ossetic = 426,
        OttomanTurkish = 427,
        Pahlavi = 428,
        PalatineGerman = 429,
        Palauan = 430,
        Pali = 431,
        Pampanga = 432,
        Pangasinan = 433,
        Papiamento = 434,
        Pashto = 435,
        PennsylvaniaGerman = 436,
        Persian = 437,
        Phoenician = 438,
        Picard = 439,
        Piedmontese = 440,
        Plautdietsch = 441,
        Pohnpeian = 442,
        Polish = 443,
        Pontic = 444,
        Portuguese = 445,
        Prussian = 446,
        Punjabi = 447,
        Quechua = 448,
        Rajasthani = 449,
        Rapanui = 450,
        Rarotongan = 451,
        Riffian = 452,
        Romagnol = 453,
        Romanian = 454,
        Romansh = 455,
        Romany = 456,
        Rombo = 457,
        Root = 458,
        Rotuman = 459,
        Roviana = 460,
        Rundi = 461,
        Russian = 462,
        Rusyn = 463,
        Rwa = 464,
        Saho = 465,
        Sakha = 466,
        SamaritanAramaic = 467,
        Samburu = 468,
        Samoan = 469,
        Samogitian = 470,
        Sandawe = 471,
        Sango = 472,
        Sangu = 473,
        Sanskrit = 474,
        Santali = 475,
        Sardinian = 476,
        Sasak = 477,
        SassareseSardinian = 478,
        SaterlandFrisian = 479,
        Saurashtra = 480,
        Scots = 481,
        ScottishGaelic = 482,
        Selayar = 483,
        Selkup = 484,
        Sena = 485,
        Seneca = 486,
        Serbian = 487,
        SerboCroatian = 488,
        Serer = 489,
        Seri = 490,
        SeselwaCreoleFrench = 491,
        Shambala = 492,
        Shan = 493,
        Shona = 494,
        SichuanYi = 495,
        Sicilian = 496,
        Sidamo = 497,
        Siksika = 498,
        Silesian = 499,
        SimplifiedChinese = 500,
        Sindhi = 501,
        Sinhala = 502,
        SkoltSami = 503,
        Slave = 504,
        Slovak = 505,
        Slovenian = 506,
        Soga = 507,
        Sogdien = 508,
        Somali = 509,
        Soninke = 510,
        SouthNdebele = 511,
        SouthernAltai = 512,
        SouthernKurdish = 513,
        SouthernSami = 514,
        SouthernSotho = 515,
        Spanish = 516,
        SrananTongo = 517,
        StandardMoroccanTamazight = 518,
        Sukuma = 519,
        Sumerian = 520,
        Sundanese = 521,
        Susu = 522,
        Swahili = 523,
        Swati = 524,
        Swedish = 525,
        SwissFrench = 526,
        SwissGerman = 527,
        SwissHighGerman = 528,
        Syriac = 529,
        Tachelhit = 530,
        Tagalog = 531,
        Tahitian = 532,
        Taita = 533,
        Tajik = 534,
        Talysh = 535,
        Tamashek = 536,
        Tamil = 537,
        Taroko = 538,
        Tasawaq = 539,
        Tatar = 540,
        Telugu = 541,
        Tereno = 542,
        Teso = 543,
        Tetum = 544,
        Thai = 545,
        Tibetan = 546,
        Tigre = 547,
        Tigrinya = 548,
        Timne = 549,
        Tiv = 550,
        Tlingit = 551,
        TokPisin = 552,
        TokelauLanguage = 553,
        Tongan = 554,
        TornedalenFinnish = 555,
        TraditionalChinese = 556,
        Tsakhur = 557,
        Tsakonian = 558,
        Tsimshian = 559,
        Tsonga = 560,
        Tswana = 561,
        Tulu = 562,
        Tumbuka = 563,
        TunisianArabic = 564,
        Turkish = 565,
        Turkmen = 566,
        Turoyo = 567,
        TuvaluLanguage = 568,
        Tuvinian = 569,
        Twi = 570,
        Tyap = 571,
        Udmurt = 572,
        Ugaritic = 573,
        Uighur = 574,
        Ukrainian = 575,
        Umbundu = 576,
        Unknownlanguage = 577,
        UpperSorbian = 578,
        Urdu = 579,
        Uzbek = 580,
        Vai = 581,
        Venda = 582,
        Venetian = 583,
        Veps = 584,
        Vietnamese = 585,
        Volapuk = 586,
        Voro = 587,
        Votic = 588,
        Vunjo = 589,
        Walloon = 590,
        Walser = 591,
        Waray = 592,
        Warlpiri = 593,
        Washo = 594,
        Wayuu = 595,
        Welsh = 596,
        WestFlemish = 597,
        WesternBalochi = 598,
        WesternFrisian = 599,
        WesternMari = 600,
        Wolaytta = 601,
        Wolof = 602,
        WuChinese = 603,
        Xhosa = 604,
        XiangChinese = 605,
        Yangben = 606,
        Yao = 607,
        Yapese = 608,
        Yemba = 609,
        Yiddish = 610,
        Yoruba = 611,
        Zapotec = 612,
        Zarma = 613,
        Zaza = 614,
        Zeelandic = 615,
        Zenaga = 616,
        Zhuang = 617,
        ZoroastrianDari = 618,
        Zulu = 619,
        Zuni = 620,

        Azeri = Azerbaijani,
        Kyrgyz = Kirghiz,
        MandarinChinese = Chinese,
        MyanmarLanguage = Burmese,
        Pushto = Pashto,
        UKEnglish = BritishEnglish,
        USEnglish = AmericanEnglish,
        Uyghur = Uighur,

        LastLanguage = Zuni
    };

    enum Country {
        AnyCountry = 0,
        Afghanistan = 1,
        Africa = 2,
        AlandIslands = 3,
        Albania = 4,
        Algeria = 5,
        AmericanSamoa = 6,
        Americas = 7,
        Andorra = 8,
        Angola = 9,
        Anguilla = 10,
        Antarctica = 11,
        AntiguaAndBarbuda = 12,
        Argentina = 13,
        Armenia = 14,
        Aruba = 15,
        AscensionIsland = 16,
        Asia = 17,
        Australasia = 18,
        Australia = 19,
        Austria = 20,
        Azerbaijan = 21,
        Bahamas = 22,
        Bahrain = 23,
        Bangladesh = 24,
        Barbados = 25,
        Belarus = 26,
        Belgium = 27,
        Belize = 28,
        Benin = 29,
        Bermuda = 30,
        Bhutan = 31,
        Bolivia = 32,
        Bosnia = 33,
        Botswana = 34,
        BouvetIsland = 35,
        Brazil = 36,
        BritishIndianOceanTerritory = 37,
        BritishVirginIslands = 38,
        Brunei = 39,
        Bulgaria = 40,
        BurkinaFaso = 41,
        Burundi = 42,
        Cambodia = 43,
        Cameroon = 44,
        Canada = 45,
        CanaryIslands = 46,
        CapeVerde = 47,
        Caribbean = 48,
        CaribbeanNetherlands = 49,
        CaymanIslands = 50,
        CentralAfricanRepublic = 51,
        CentralAmerica = 52,
        CentralAsia = 53,
        CeutaAndMelilla = 54,
        Chad = 55,
        Chile = 56,
        China = 57,
        ChristmasIsland = 58,
        ClippertonIsland = 59,
        CocosKeelingIslands = 60,
        Colombia = 61,
        Comoros = 62,
        CongoBrazzaville = 63,
        CongoDRC = 64,
        CookIslands = 65,
        CostaRica = 66,
        CotedIvoire = 67,
        Croatia = 68,
        Cuba = 69,
        Curacao = 70,
        Cyprus = 71,
        CzechRepublic = 72,
        Denmark = 73,
        DiegoGarcia = 74,
        Djibouti = 75,
        Dominica = 76,
        DominicanRepublic = 77,
        EastTimor = 78,
        EasternAfrica = 79,
        EasternAsia = 80,
        EasternEurope = 81,
        Ecuador = 82,
        Egypt = 83,
        ElSalvador = 84,
        EquatorialGuinea = 85,
        Eritrea = 86,
        Estonia = 87,
        Eswatini = 88,
        Ethiopia = 89,
        Europe = 90,
        EuropeanUnion = 91,
        Eurozone = 92,
        FalklandIslands = 93,
        FaroeIslands = 94,
        Fiji = 95,
        Finland = 96,
        France = 97,
        FrenchGuiana = 98,
        FrenchPolynesia = 99,
        FrenchSouthernTerritories = 100,
        Gabon = 101,
        Gambia = 102,
        Georgia = 103,
        Germany = 104,
        Ghana = 105,
        Gibraltar = 106,
        Greece = 107,
        Greenland = 108,
        Grenada = 109,
        Guadeloupe = 110,
        Guam = 111,
        Guatemala = 112,
        Guernsey = 113,
        Guinea = 114,
        GuineaBissau = 115,
        Guyana = 116,
        Haiti = 117,
        HeardAndMcDonaldIslands = 118,
        Honduras = 119,
        HongKong = 120,
        Hungary = 121,
        Iceland = 122,
        India = 123,
        Indonesia = 124,
        Iran = 125,
        Iraq = 126,
        Ireland = 127,
        IsleofMan = 128,
        Israel = 129,
        Italy = 130,
        Jamaica = 131,
        Japan = 132,
        Jersey = 133,
        Jordan = 134,
        Kazakhstan = 135,
        Kenya = 136,
        Kiribati = 137,
        Kosovo = 138,
        Kuwait = 139,
        Kyrgyzstan = 140,
        Laos = 141,
        LatinAmerica = 142,
        Latvia = 143,
        Lebanon = 144,
        Lesotho = 145,
        Liberia = 146,
        Libya = 147,
        Liechtenstein = 148,
        Lithuania = 149,
        Luxembourg = 150,
        Macao = 151,
        Madagascar = 152,
        Malawi = 153,
        Malaysia = 154,
        Maldives = 155,
        Mali = 156,
        Malta = 157,
        MarshallIslands = 158,
        Martinique = 159,
        Mauritania = 160,
        Mauritius = 161,
        Mayotte = 162,
        Melanesia = 163,
        Mexico = 164,
        Micronesia = 165,
        MicronesianRegion = 166,
        MiddleAfrica = 167,
        Moldova = 168,
        Monaco = 169,
        Mongolia = 170,
        Montenegro = 171,
        Montserrat = 172,
        Morocco = 173,
        Mozambique = 174,
        Myanmar = 175,
        Namibia = 176,
        Nauru = 177,
        Nepal = 178,
        Netherlands = 179,
        NewCaledonia = 180,
        NewZealand = 181,
        Nicaragua = 182,
        Niger = 183,
        Nigeria = 184,
        Niue = 185,
        NorfolkIsland = 186,
        NorthAmerica = 187,
        NorthKorea = 188,
        NorthMacedonia = 189,
        NorthernAfrica = 190,
        NorthernAmerica = 191,
        NorthernEurope = 192,
        NorthernMarianaIslands = 193,
        Norway = 194,
        Oceania = 195,
        Oman = 196,
        OutlyingOceania = 197,
        Pakistan = 198,
        Palau = 199,
        Palestine = 200,
        Panama = 201,
        PapuaNewGuinea = 202,
        Paraguay = 203,
        Peru = 204,
        Philippines = 205,
        PitcairnIslands = 206,
        Poland = 207,
        Polynesia = 208,
        Portugal = 209,
        PseudoAccents = 210,
        PseudoBidi = 211,
        PuertoRico = 212,
        Qatar = 213,
        Reunion = 214,
        Romania = 215,
        Russia = 216,
        Rwanda = 217,
        Samoa = 218,
        SanMarino = 219,
        SaoTomeAndPrincipe = 220,
        SaudiArabia = 221,
        Senegal = 222,
        Serbia = 223,
        Seychelles = 224,
        SierraLeone = 225,
        Singapore = 226,
        SintMaarten = 227,
        Slovakia = 228,
        Slovenia = 229,
        SolomonIslands = 230,
        Somalia = 231,
        SouthAfrica = 232,
        SouthAmerica = 233,
        SouthGeorgiaAndSouthSandwichIslands = 234,
        SouthKorea = 235,
        SouthSudan = 236,
        SoutheastAsia = 237,
        SouthernAfrica = 238,
        SouthernAsia = 239,
        SouthernEurope = 240,
        Spain = 241,
        SriLanka = 242,
        StBarthelemy = 243,
        StHelena = 244,
        StKittsAndNevis = 245,
        StLucia = 246,
        StMartin = 247,
        StPierreAndMiquelon = 248,
        StVincentAndGrenadines = 249,
        SubSaharanAfrica = 250,
        Sudan = 251,
        Suriname = 252,
        SvalbardAndJanMayen = 253,
        Sweden = 254,
        Switzerland = 255,
        Syria = 256,
        Taiwan = 257,
        Tajikistan = 258,
        Tanzania = 259,
        Thailand = 260,
        Togo = 261,
        Tokelau = 262,
        Tonga = 263,
        TrinidadAndTobago = 264,
        TristandaCunha = 265,
        Tunisia = 266,
        Turkey = 267,
        Turkmenistan = 268,
        TurksAndCaicosIslands = 269,
        Tuvalu = 270,
        UK = 271,
        UN = 272,
        US = 273,
        Uganda = 274,
        Ukraine = 275,
        UnitedArabEmirates = 276,
        UnitedStatesOutlyingIslands = 277,
        UnitedStatesVirginIslands = 278,
        UnknownRegion = 279,
        Uruguay = 280,
        Uzbekistan = 281,
        Vanuatu = 282,
        VaticanCity = 283,
        Venezuela = 284,
        Vietnam = 285,
        WallisAndFutuna = 286,
        WesternAfrica = 287,
        WesternAsia = 288,
        WesternEurope = 289,
        WesternSahara = 290,
        World = 291,
        Yemen = 292,
        Zambia = 293,
        Zimbabwe = 294,

        BosniaAndHerzegovina = Bosnia,
        CongoKinshasa = CongoDRC,
        CongoRepublic = CongoBrazzaville,
        Czechia = CzechRepublic,
        FalklandIslandsIslasMalvinas = FalklandIslands,
        HongKongSARChina = HongKong,
        IvoryCoast = CotedIvoire,
        MacaoSARChina = Macao,
        MyanmarBurma = Myanmar,
        PalestinianTerritories = Palestine,
        Swaziland = Eswatini,
        TimorLeste = EastTimor,
        UnitedKingdom = UK,
        UnitedNations = UN,
        UnitedStates = US,

        LastCountry = Zimbabwe
    };

    enum Script {
        AnyScript = 0,
        AdlamScript = 1,
        AfakaScript = 2,
        AhomScript = 3,
        AnatolianHieroglyphsScript = 4,
        ArabicScript = 5,
        ArmenianScript = 6,
        AvestanScript = 7,
        BalineseScript = 8,
        BamumScript = 9,
        BanglaScript = 10,
        BassaVahScript = 11,
        BatakScript = 12,
        BhaiksukiScript = 13,
        BlissymbolsScript = 14,
        BookPahlaviScript = 15,
        BopomofoScript = 16,
        BrahmiScript = 17,
        BrailleScript = 18,
        BugineseScript = 19,
        BuhidScript = 20,
        CarianScript = 21,
        CaucasianAlbanianScript = 22,
        ChakmaScript = 23,
        ChamScript = 24,
        CherokeeScript = 25,
        CirthScript = 26,
        CopticScript = 27,
        CypriotScript = 28,
        CyrillicScript = 29,
        DeseretScript = 30,
        DevanagariScript = 31,
        DograScript = 32,
        DuployanshorthandScript = 33,
        EasternSyriacScript = 34,
        EgyptiandemoticScript = 35,
        EgyptianhieraticScript = 36,
        EgyptianhieroglyphsScript = 37,
        ElbasanScript = 38,
        ElymaicScript = 39,
        EmojiScript = 40,
        EstrangeloSyriacScript = 41,
        EthiopicScript = 42,
        FrakturLatinScript = 43,
        FraserScript = 44,
        GaelicLatinScript = 45,
        GeorgianKhutsuriScript = 46,
        GeorgianScript = 47,
        GlagoliticScript = 48,
        GothicScript = 49,
        GranthaScript = 50,
        GreekScript = 51,
        GujaratiScript = 52,
        GunjalaGondiScript = 53,
        GurmukhiScript = 54,
        HanScript = 55,
        HangulScript = 56,
        HanifiRohingyaScript = 57,
        HanunooScript = 58,
        HanwithBopomofoScript = 59,
        HatranScript = 60,
        HebrewScript = 61,
        HiraganaScript = 62,
        ImperialAramaicScript = 63,
        IndusScript = 64,
        InheritedScript = 65,
        InscriptionalPahlaviScript = 66,
        InscriptionalParthianScript = 67,
        JamoScript = 68,
        JapaneseScript = 69,
        JapanesesyllabariesScript = 70,
        JavaneseScript = 71,
        JurchenScript = 72,
        KaithiScript = 73,
        KannadaScript = 74,
        KatakanaScript = 75,
        KayahLiScript = 76,
        KharoshthiScript = 77,
        KhmerScript = 78,
        KhojkiScript = 79,
        KhudawadiScript = 80,
        KoreanScript = 81,
        KpelleScript = 82,
        LannaScript = 83,
        LaoScript = 84,
        LatinScript = 85,
        LepchaScript = 86,
        LimbuScript = 87,
        LinearAScript = 88,
        LinearBScript = 89,
        LomaScript = 90,
        LycianScript = 91,
        LydianScript = 92,
        MahajaniScript = 93,
        MakasarScript = 94,
        MalayalamScript = 95,
        MandaeanScript = 96,
        ManichaeanScript = 97,
        MarchenScript = 98,
        MasaramGondiScript = 99,
        MathematicalNotationScript = 100,
        MayanhieroglyphsScript = 101,
        MedefaidrinScript = 102,
        MeiteiMayekScript = 103,
        MendeScript = 104,
        MeroiticCursiveScript = 105,
        MeroiticScript = 106,
        ModiScript = 107,
        MongolianScript = 108,
        MoonScript = 109,
        MroScript = 110,
        MultaniScript = 111,
        MyanmarScript = 112,
        NKoScript = 113,
        NabataeanScript = 114,
        NandinagariScript = 115,
        NaxiGebaScript = 116,
        NewTaiLueScript = 117,
        NewaScript = 118,
        NushuScript = 119,
        NyiakengPuachueHmongScript = 120,
        OdiaScript = 121,
        OghamScript = 122,
        OlChikiScript = 123,
        OldChurchSlavonicCyrillicScript = 124,
        OldHungarianScript = 125,
        OldItalicScript = 126,
        OldNorthArabianScript = 127,
        OldPermicScript = 128,
        OldPersianScript = 129,
        OldSogdianScript = 130,
        OldSouthArabianScript = 131,
        OrkhonScript = 132,
        OsageScript = 133,
        OsmanyaScript = 134,
        PahawhHmongScript = 135,
        PalmyreneScript = 136,
        PauCinHauScript = 137,
        PhagspaScript = 138,
        PhoenicianScript = 139,
        PollardPhoneticScript = 140,
        PsalterPahlaviScript = 141,
        RejangScript = 142,
        RongorongoScript = 143,
        RunicScript = 144,
        SACuneiformScript = 145,
        SamaritanScript = 146,
        SaratiScript = 147,
        SaurashtraScript = 148,
        SharadaScript = 149,
        ShavianScript = 150,
        SiddhamScript = 151,
        SignWritingScript = 152,
        SimplifiedHanScript = 153,
        SinhalaScript = 154,
        SogdianScript = 155,
        SoraSompengScript = 156,
        SoyomboScript = 157,
        SundaneseScript = 158,
        SylotiNagriScript = 159,
        SymbolsScript = 160,
        SyriacScript = 161,
        TagalogScript = 162,
        TagbanwaScript = 163,
        TaiLeScript = 164,
        TaiVietScript = 165,
        TakriScript = 166,
        TamilScript = 167,
        TangutScript = 168,
        TeluguScript = 169,
        TengwarScript = 170,
        ThaanaScript = 171,
        ThaiScript = 172,
        TibetanScript = 173,
        TifinaghScript = 174,
        TirhutaScript = 175,
        TraditionalHanScript = 176,
        UCASScript = 177,
        UgariticScript = 178,
        UnwrittenScript = 179,
        VaiScript = 180,
        VarangKshitiScript = 181,
        VisibleSpeechScript = 182,
        WanchoScript = 183,
        WesternSyriacScript = 184,
        WoleaiScript = 185,
        YiScript = 186,
        ZanabazarSquareScript = 187,

        PersoArabicScript = ArabicScript,
        SimplifiedScript = SimplifiedHanScript,
        SumeroAkkadianCuneiformScript = SACuneiformScript,
        TraditionalScript = TraditionalHanScript,
        UnifiedCanadianAboriginalSyllabicsScript = UCASScript,

        LastScript = ZanabazarSquareScript
    };
// GENERATED PART ENDS HERE

    enum MeasurementSystem { MetricSystem, ImperialSystem };

    enum FormatType { LongFormat, ShortFormat, NarrowFormat };
    enum NumberOption {
        OmitGroupSeparator = 0x01,
        RejectGroupSeparator = 0x02
    };
    Q_DECLARE_FLAGS(NumberOptions, NumberOption)

    enum CurrencySymbolFormat {
        CurrencyIsoCode,
        CurrencySymbol,
        CurrencyDisplayName
    };

    QLocale();
    QLocale(const QString &name);
    QLocale(Language language, Country country = AnyCountry);
    QLocale(Language language, Script script, Country country);
    QLocale(const QLocale &other);

    QLocale &operator=(const QLocale &other);

    Language language() const;
    Script script() const;
    Country country() const;
    QString name() const;

    QString bcp47Name() const;
    QString nativeLanguageName() const;
    QString nativeCountryName() const;

    short toShort(const QString &s, bool *ok = Q_NULLPTR, int base = 0) const;
    ushort toUShort(const QString &s, bool *ok = Q_NULLPTR, int base = 0) const;
    int toInt(const QString &s, bool *ok = Q_NULLPTR, int base = 0) const;
    uint toUInt(const QString &s, bool *ok = Q_NULLPTR, int base = 0) const;
    qlonglong toLongLong(const QString &s, bool *ok = Q_NULLPTR, int base = 0) const;
    qulonglong toULongLong(const QString &s, bool *ok = Q_NULLPTR, int base = 0) const;
    float toFloat(const QString &s, bool *ok = Q_NULLPTR) const;
    double toDouble(const QString &s, bool *ok = Q_NULLPTR) const;

    QString toString(qlonglong i) const;
    QString toString(qulonglong i) const;
    inline QString toString(short i) const;
    inline QString toString(ushort i) const;
    inline QString toString(int i) const;
    inline QString toString(uint i) const;
    QString toString(double i, char f = 'g', int prec = 6) const;
    inline QString toString(float i, char f = 'g', int prec = 6) const;
    QString toString(const QDate &date, const QString &formatStr) const;
    QString toString(const QDate &date, FormatType format = LongFormat) const;
    QString toString(const QTime &time, const QString &formatStr) const;
    QString toString(const QTime &time, FormatType format = LongFormat) const;
    QString toString(const QDateTime &dateTime, FormatType format = LongFormat) const;
    QString toString(const QDateTime &dateTime, const QString &format) const;

    QString dateFormat(FormatType format = LongFormat) const;
    QString timeFormat(FormatType format = LongFormat) const;
    QString dateTimeFormat(FormatType format = LongFormat) const;
#ifndef QT_NO_DATESTRING
    QDate toDate(const QString &string, FormatType = LongFormat) const;
    QTime toTime(const QString &string, FormatType = LongFormat) const;
    QDateTime toDateTime(const QString &string, FormatType format = LongFormat) const;
    QDate toDate(const QString &string, const QString &format) const;
    QTime toTime(const QString &string, const QString &format) const;
    QDateTime toDateTime(const QString &string, const QString &format) const;
#endif

    // ### Qt 5: We need to return QString from these function since
    //           unicode data contains several characters for these fields.
    QChar decimalPoint() const;
    QChar groupSeparator() const;
    QChar percent() const;
    QChar zeroDigit() const;
    QChar negativeSign() const;
    QChar positiveSign() const;
    QChar exponential() const;

    QString monthName(int, FormatType format = LongFormat) const;
    QString standaloneMonthName(int, FormatType format = LongFormat) const;
    QString dayName(int, FormatType format = LongFormat) const;
    QString standaloneDayName(int, FormatType format = LongFormat) const;

    Qt::DayOfWeek firstDayOfWeek() const;
    QList<Qt::DayOfWeek> weekdays() const;

    QString amText() const;
    QString pmText() const;

    MeasurementSystem measurementSystem() const;

    Qt::LayoutDirection textDirection() const;

    QString toUpper(const QString &str) const;
    QString toLower(const QString &str) const;

    QString currencySymbol(CurrencySymbolFormat = CurrencySymbol) const;
    QString toCurrencyString(qlonglong, const QString &symbol = QString()) const;
    QString toCurrencyString(qulonglong, const QString &symbol = QString()) const;
    inline QString toCurrencyString(short, const QString &symbol = QString()) const;
    inline QString toCurrencyString(ushort, const QString &symbol = QString()) const;
    inline QString toCurrencyString(int, const QString &symbol = QString()) const;
    inline QString toCurrencyString(uint, const QString &symbol = QString()) const;
    QString toCurrencyString(double, const QString &symbol = QString()) const;
    inline QString toCurrencyString(float, const QString &symbol = QString()) const;

    QStringList uiLanguages() const;

    inline bool operator==(const QLocale &other) const;
    inline bool operator!=(const QLocale &other) const;

    static QString languageToString(Language language);
    static QString countryToString(Country country);
    static QString scriptToString(Script script);
    static void setDefault(const QLocale &locale);

    static QLocale c() { return QLocale(C); }
    static QLocale system();

    static QList<QLocale> matchingLocales(QLocale::Language language, QLocale::Script script, QLocale::Country country);
    static QList<Country> countriesForLanguage(Language lang);

    void setNumberOptions(NumberOptions options);
    NumberOptions numberOptions() const;

    enum QuotationStyle { StandardQuotation, AlternateQuotation };
    QString quoteString(const QString &str, QuotationStyle style = StandardQuotation) const;
    QString quoteString(const QStringRef &str, QuotationStyle style = StandardQuotation) const;

    QString createSeparatedList(const QStringList &strl) const;
private:
    friend struct QLocalePrivate;
    // ### We now use this field to pack an index into locale_data and NumberOptions.
    // ### Qt 5: change to a QLocaleData *d; uint numberOptions.
    struct Data {
        quint16 index;
        QLocale::NumberOptions numberOptions;
    };

    Data p;
    const QLocalePrivate *d() const;
};
Q_DECLARE_TYPEINFO(QLocale, Q_MOVABLE_TYPE);
Q_DECLARE_OPERATORS_FOR_FLAGS(QLocale::NumberOptions)

inline QString QLocale::toString(short i) const
    { return toString(qlonglong(i)); }
inline QString QLocale::toString(ushort i) const
    { return toString(qulonglong(i)); }
inline QString QLocale::toString(int i) const
    { return toString(qlonglong(i)); }
inline QString QLocale::toString(uint i) const
    { return toString(qulonglong(i)); }
inline QString QLocale::toString(float i, char f, int prec) const
    { return toString(double(i), f, prec); }
inline bool QLocale::operator==(const QLocale &other) const
    { return d() == other.d() && numberOptions() == other.numberOptions(); }
inline bool QLocale::operator!=(const QLocale &other) const
    { return d() != other.d() || numberOptions() != other.numberOptions(); }

inline QString QLocale::toCurrencyString(short i, const QString &symbol) const
    { return toCurrencyString(qlonglong(i), symbol); }
inline QString QLocale::toCurrencyString(ushort i, const QString &symbol) const
    { return toCurrencyString(qulonglong(i), symbol); }
inline QString QLocale::toCurrencyString(int i, const QString &symbol) const
{ return toCurrencyString(qlonglong(i), symbol); }
inline QString QLocale::toCurrencyString(uint i, const QString &symbol) const
{ return toCurrencyString(qulonglong(i), symbol); }
inline QString QLocale::toCurrencyString(float i, const QString &symbol) const
{ return toCurrencyString(double(i), symbol); }

#ifndef QT_NO_DATASTREAM
Q_CORE_EXPORT QDataStream &operator<<(QDataStream &, const QLocale &);
Q_CORE_EXPORT QDataStream &operator>>(QDataStream &, QLocale &);
#endif

QT_END_NAMESPACE

#ifndef QT_NO_SYSTEMLOCALE
Q_DECLARE_METATYPE(QSystemLocale::CurrencyToStringArgument)
#endif

QT_END_HEADER

#endif // QLOCALE_H
