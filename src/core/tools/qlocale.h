/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2021 Ivailo Monev
**
** This file is part of the QtCore module of the Katie Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
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

class QLocalePrivate;
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
        ArabicNajdi = 27,
        Aragonese = 28,
        Aramaic = 29,
        Araona = 30,
        Arapaho = 31,
        Arawak = 32,
        Armenian = 33,
        Aromanian = 34,
        Arpitan = 35,
        Assamese = 36,
        Asturian = 37,
        Asu = 38,
        Atsam = 39,
        AustralianEnglish = 40,
        AustrianGerman = 41,
        Avaric = 42,
        Avestan = 43,
        Awadhi = 44,
        Aymara = 45,
        Azerbaijani = 46,
        Badaga = 47,
        Bafia = 48,
        Bafut = 49,
        Bakhtiari = 50,
        Balinese = 51,
        Baluchi = 52,
        Bambara = 53,
        Bamun = 54,
        Bangla = 55,
        Banjar = 56,
        Basaa = 57,
        Bashkir = 58,
        Basque = 59,
        BatakToba = 60,
        Bavarian = 61,
        Beja = 62,
        Belarusian = 63,
        Bemba = 64,
        Bena = 65,
        Betawi = 66,
        Bhojpuri = 67,
        Bikol = 68,
        Bini = 69,
        Bishnupriya = 70,
        Bislama = 71,
        Blin = 72,
        Blissymbols = 73,
        Bodo = 74,
        Bosnian = 75,
        Brahui = 76,
        Braj = 77,
        BrazilianPortuguese = 78,
        Breton = 79,
        BritishEnglish = 80,
        Buginese = 81,
        Bulgarian = 82,
        Bulu = 83,
        Buriat = 84,
        Burmese = 85,
        Caddo = 86,
        CajunFrench = 87,
        CanadianEnglish = 88,
        CanadianFrench = 89,
        Cantonese = 90,
        Capiznon = 91,
        Carib = 92,
        Catalan = 93,
        Cayuga = 94,
        Cebuano = 95,
        CentralAtlasTamazight = 96,
        CentralDusun = 97,
        CentralKurdish = 98,
        CentralYupik = 99,
        ChadianArabic = 100,
        Chagatai = 101,
        Chakma = 102,
        Chamorro = 103,
        Chechen = 104,
        Cherokee = 105,
        Cheyenne = 106,
        Chibcha = 107,
        Chickasaw = 108,
        Chiga = 109,
        ChimborazoHighlandQuichua = 110,
        Chinese = 111,
        ChinookJargon = 112,
        Chipewyan = 113,
        Choctaw = 114,
        ChurchSlavic = 115,
        Chuukese = 116,
        Chuvash = 117,
        ClassicalNewari = 118,
        ClassicalSyriac = 119,
        Colognian = 120,
        Comorian = 121,
        CongoSwahili = 122,
        Coptic = 123,
        Cornish = 124,
        Corsican = 125,
        Cree = 126,
        CrimeanTurkish = 127,
        Croatian = 128,
        Czech = 129,
        Dakota = 130,
        Danish = 131,
        Dargwa = 132,
        Dari = 133,
        Dazaga = 134,
        Delaware = 135,
        Dinka = 136,
        Divehi = 137,
        Dogri = 138,
        Dogrib = 139,
        Duala = 140,
        Dutch = 141,
        Dyula = 142,
        Dzongkha = 143,
        EasternFrisian = 144,
        Efik = 145,
        EgyptianArabic = 146,
        Ekajuk = 147,
        Elamite = 148,
        Embu = 149,
        Emilian = 150,
        English = 151,
        Erzya = 152,
        Esperanto = 153,
        Estonian = 154,
        EuropeanPortuguese = 155,
        EuropeanSpanish = 156,
        Ewe = 157,
        Ewondo = 158,
        Extremaduran = 159,
        Fang = 160,
        Fanti = 161,
        Faroese = 162,
        FijiHindi = 163,
        Fijian = 164,
        Filipino = 165,
        Finnish = 166,
        Flemish = 167,
        Fon = 168,
        Frafra = 169,
        French = 170,
        Friulian = 171,
        Fulah = 172,
        Ga = 173,
        Gagauz = 174,
        Galician = 175,
        GanChinese = 176,
        Ganda = 177,
        Gayo = 178,
        Gbaya = 179,
        Geez = 180,
        Georgian = 181,
        German = 182,
        GhegAlbanian = 183,
        Ghomala = 184,
        Gilaki = 185,
        Gilbertese = 186,
        GoanKonkani = 187,
        Gondi = 188,
        Gorontalo = 189,
        Gothic = 190,
        Grebo = 191,
        Greek = 192,
        Guarani = 193,
        Gujarati = 194,
        Gusii = 195,
        Gwichin = 196,
        Haida = 197,
        HaitianCreole = 198,
        HakkaChinese = 199,
        Hausa = 200,
        Hawaiian = 201,
        Hebrew = 202,
        Herero = 203,
        Hiligaynon = 204,
        Hindi = 205,
        HiriMotu = 206,
        Hittite = 207,
        Hmong = 208,
        Hungarian = 209,
        Hupa = 210,
        Iban = 211,
        Ibibio = 212,
        Icelandic = 213,
        Ido = 214,
        Igbo = 215,
        Iloko = 216,
        InariSami = 217,
        Indonesian = 218,
        Ingrian = 219,
        Ingush = 220,
        Interlingua = 221,
        Interlingue = 222,
        Inuktitut = 223,
        Inupiaq = 224,
        Irish = 225,
        Italian = 226,
        JamaicanCreoleEnglish = 227,
        Japanese = 228,
        Javanese = 229,
        Jju = 230,
        JolaFonyi = 231,
        JudeoArabic = 232,
        JudeoPersian = 233,
        Jutish = 234,
        Kabardian = 235,
        Kabuverdianu = 236,
        Kabyle = 237,
        Kachin = 238,
        Kaingang = 239,
        Kako = 240,
        Kalaallisut = 241,
        Kalenjin = 242,
        Kalmyk = 243,
        Kamba = 244,
        Kanembu = 245,
        Kannada = 246,
        Kanuri = 247,
        KaraKalpak = 248,
        KarachayBalkar = 249,
        Karelian = 250,
        Kashmiri = 251,
        Kashubian = 252,
        Kawi = 253,
        Kazakh = 254,
        Kenyang = 255,
        Khasi = 256,
        Khmer = 257,
        Khotanese = 258,
        Khowar = 259,
        Kiche = 260,
        Kikuyu = 261,
        Kimbundu = 262,
        Kinaraya = 263,
        Kinyarwanda = 264,
        Kirghiz = 265,
        Kirmanjki = 266,
        Klingon = 267,
        Kom = 268,
        Komi = 269,
        KomiPermyak = 270,
        Kongo = 271,
        Konkani = 272,
        Korean = 273,
        Koro = 274,
        Kosraean = 275,
        Kotava = 276,
        KoyraChiini = 277,
        KoyraboroSenni = 278,
        Kpelle = 279,
        Krio = 280,
        Kuanyama = 281,
        Kumyk = 282,
        Kurdish = 283,
        Kurukh = 284,
        Kutenai = 285,
        Kwasio = 286,
        Ladino = 287,
        Lahnda = 288,
        Lakota = 289,
        Lamba = 290,
        Langi = 291,
        Lao = 292,
        Latgalian = 293,
        Latin = 294,
        LatinAmericanSpanish = 295,
        Latvian = 296,
        Laz = 297,
        Lezghian = 298,
        Ligurian = 299,
        Limburgish = 300,
        Lingala = 301,
        LinguaFrancaNova = 302,
        LiteraryChinese = 303,
        Lithuanian = 304,
        Livonian = 305,
        Lojban = 306,
        Lombard = 307,
        LouisianaCreole = 308,
        LowGerman = 309,
        LowSaxon = 310,
        LowerSilesian = 311,
        LowerSorbian = 312,
        Lozi = 313,
        LubaKatanga = 314,
        LubaLulua = 315,
        Luiseno = 316,
        LuleSami = 317,
        Lunda = 318,
        Luo = 319,
        Luxembourgish = 320,
        Luyia = 321,
        Maba = 322,
        Macedonian = 323,
        Machame = 324,
        Madurese = 325,
        Mafa = 326,
        Magahi = 327,
        MainFranconian = 328,
        Maithili = 329,
        Makasar = 330,
        MakhuwaMeetto = 331,
        Makonde = 332,
        Malagasy = 333,
        Malay = 334,
        Malayalam = 335,
        Maltese = 336,
        Manchu = 337,
        Mandar = 338,
        Mandingo = 339,
        Manipuri = 340,
        Manx = 341,
        Maori = 342,
        Mapuche = 343,
        Marathi = 344,
        Mari = 345,
        Marshallese = 346,
        Marwari = 347,
        Masai = 348,
        Mazanderani = 349,
        Medumba = 350,
        Mende = 351,
        Mentawai = 352,
        Meru = 353,
        Meta = 354,
        MexicanSpanish = 355,
        MiddleDutch = 356,
        MiddleEnglish = 357,
        MiddleFrench = 358,
        MiddleHighGerman = 359,
        MiddleIrish = 360,
        Mikmaq = 361,
        MinNanChinese = 362,
        Minangkabau = 363,
        Mingrelian = 364,
        Mirandese = 365,
        Mizo = 366,
        ModernStandardArabic = 367,
        Mohawk = 368,
        Moksha = 369,
        Moldavian = 370,
        Mongo = 371,
        Mongolian = 372,
        Montenegrin = 373,
        Morisyen = 374,
        MoroccanArabic = 375,
        Mossi = 376,
        Multiplelanguages = 377,
        Mundang = 378,
        Muscogee = 379,
        MuslimTat = 380,
        Myene = 381,
        NKo = 382,
        Nama = 383,
        NauruLanguage = 384,
        Navajo = 385,
        Ndonga = 386,
        Neapolitan = 387,
        Nepali = 388,
        Newari = 389,
        Ngambay = 390,
        Ngiemboon = 391,
        Ngomba = 392,
        Nheengatu = 393,
        Nias = 394,
        NigerianPidgin = 395,
        Niuean = 396,
        Nogai = 397,
        Nolinguisticcontent = 398,
        NorthNdebele = 399,
        NorthernFrisian = 400,
        NorthernLuri = 401,
        NorthernSami = 402,
        NorthernSotho = 403,
        Norwegian = 404,
        NorwegianBokmal = 405,
        NorwegianNynorsk = 406,
        Novial = 407,
        Nuer = 408,
        Nyamwezi = 409,
        Nyanja = 410,
        Nyankole = 411,
        NyasaTonga = 412,
        Nyoro = 413,
        Nzima = 414,
        Occitan = 415,
        Odia = 416,
        Ojibwa = 417,
        OldEnglish = 418,
        OldFrench = 419,
        OldHighGerman = 420,
        OldIrish = 421,
        OldNorse = 422,
        OldPersian = 423,
        OldProvencal = 424,
        Oromo = 425,
        Osage = 426,
        Ossetic = 427,
        OttomanTurkish = 428,
        Pahlavi = 429,
        PalatineGerman = 430,
        Palauan = 431,
        Pali = 432,
        Pampanga = 433,
        Pangasinan = 434,
        Papiamento = 435,
        Pashto = 436,
        PennsylvaniaGerman = 437,
        Persian = 438,
        Phoenician = 439,
        Picard = 440,
        Piedmontese = 441,
        Plautdietsch = 442,
        Pohnpeian = 443,
        Polish = 444,
        Pontic = 445,
        Portuguese = 446,
        Prussian = 447,
        Punjabi = 448,
        Quechua = 449,
        Rajasthani = 450,
        Rapanui = 451,
        Rarotongan = 452,
        Riffian = 453,
        Romagnol = 454,
        Romanian = 455,
        Romansh = 456,
        Romany = 457,
        Rombo = 458,
        Root = 459,
        Rotuman = 460,
        Roviana = 461,
        Rundi = 462,
        Russian = 463,
        Rusyn = 464,
        Rwa = 465,
        Saho = 466,
        Sakha = 467,
        SamaritanAramaic = 468,
        Samburu = 469,
        Samoan = 470,
        Samogitian = 471,
        Sandawe = 472,
        Sango = 473,
        Sangu = 474,
        Sanskrit = 475,
        Santali = 476,
        Sardinian = 477,
        Sasak = 478,
        SassareseSardinian = 479,
        SaterlandFrisian = 480,
        Saurashtra = 481,
        Scots = 482,
        ScottishGaelic = 483,
        Selayar = 484,
        Selkup = 485,
        Sena = 486,
        Seneca = 487,
        Serbian = 488,
        SerboCroatian = 489,
        Serer = 490,
        Seri = 491,
        SeselwaCreoleFrench = 492,
        Shambala = 493,
        Shan = 494,
        Shona = 495,
        SichuanYi = 496,
        Sicilian = 497,
        Sidamo = 498,
        Siksika = 499,
        Silesian = 500,
        SimplifiedChinese = 501,
        Sindhi = 502,
        Sinhala = 503,
        SkoltSami = 504,
        Slave = 505,
        Slovak = 506,
        Slovenian = 507,
        Soga = 508,
        Sogdien = 509,
        Somali = 510,
        Soninke = 511,
        SouthNdebele = 512,
        SouthernAltai = 513,
        SouthernKurdish = 514,
        SouthernSami = 515,
        SouthernSotho = 516,
        Spanish = 517,
        SrananTongo = 518,
        StandardMoroccanTamazight = 519,
        Sukuma = 520,
        Sumerian = 521,
        Sundanese = 522,
        Susu = 523,
        Swahili = 524,
        Swati = 525,
        Swedish = 526,
        SwissFrench = 527,
        SwissGerman = 528,
        SwissHighGerman = 529,
        Syriac = 530,
        Tachelhit = 531,
        Tagalog = 532,
        Tahitian = 533,
        Taita = 534,
        Tajik = 535,
        Talysh = 536,
        Tamashek = 537,
        Tamil = 538,
        Taroko = 539,
        Tasawaq = 540,
        Tatar = 541,
        Telugu = 542,
        Tereno = 543,
        Teso = 544,
        Tetum = 545,
        Thai = 546,
        Tibetan = 547,
        Tigre = 548,
        Tigrinya = 549,
        Timne = 550,
        Tiv = 551,
        Tlingit = 552,
        TokPisin = 553,
        TokelauLanguage = 554,
        Tongan = 555,
        TornedalenFinnish = 556,
        TraditionalChinese = 557,
        Tsakhur = 558,
        Tsakonian = 559,
        Tsimshian = 560,
        Tsonga = 561,
        Tswana = 562,
        Tulu = 563,
        Tumbuka = 564,
        TunisianArabic = 565,
        Turkish = 566,
        Turkmen = 567,
        Turoyo = 568,
        TuvaluLanguage = 569,
        Tuvinian = 570,
        Twi = 571,
        Tyap = 572,
        Udmurt = 573,
        Ugaritic = 574,
        Uighur = 575,
        Ukrainian = 576,
        Umbundu = 577,
        Unknownlanguage = 578,
        UpperSorbian = 579,
        Urdu = 580,
        Uzbek = 581,
        Vai = 582,
        Venda = 583,
        Venetian = 584,
        Veps = 585,
        Vietnamese = 586,
        Volapuk = 587,
        Voro = 588,
        Votic = 589,
        Vunjo = 590,
        Walloon = 591,
        Walser = 592,
        Waray = 593,
        Warlpiri = 594,
        Washo = 595,
        Wayuu = 596,
        Welsh = 597,
        WestFlemish = 598,
        WesternBalochi = 599,
        WesternFrisian = 600,
        WesternMari = 601,
        Wolaytta = 602,
        Wolof = 603,
        WuChinese = 604,
        Xhosa = 605,
        XiangChinese = 606,
        Yangben = 607,
        Yao = 608,
        Yapese = 609,
        Yemba = 610,
        Yiddish = 611,
        Yoruba = 612,
        Zapotec = 613,
        Zarma = 614,
        Zaza = 615,
        Zeelandic = 616,
        Zenaga = 617,
        Zhuang = 618,
        ZoroastrianDari = 619,
        Zulu = 620,
        Zuni = 621,

        Azeri = Azerbaijani,
        ChineseCantonese = Cantonese,
        ChineseMandarin = Chinese,
        KurdishCentral = CentralKurdish,
        KurdishSorani = CentralKurdish,
        Kyrgyz = Kirghiz,
        MandarinChinese = Chinese,
        MyanmarLanguage = Burmese,
        NajdiArabic = ArabicNajdi,
        Pushto = Pashto,
        SimplifiedMandarinChinese = SimplifiedChinese,
        TraditionalMandarinChinese = TraditionalChinese,
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
        CaboVerde = 43,
        Cambodia = 44,
        Cameroon = 45,
        Canada = 46,
        CanaryIslands = 47,
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
        CapeVerde = CaboVerde,
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
        ChorasmianScript = 26,
        CirthScript = 27,
        CopticScript = 28,
        CypriotScript = 29,
        CyrillicScript = 30,
        DeseretScript = 31,
        DevanagariScript = 32,
        DivesAkuruScript = 33,
        DograScript = 34,
        DuployanshorthandScript = 35,
        EasternSyriacScript = 36,
        EgyptiandemoticScript = 37,
        EgyptianhieraticScript = 38,
        EgyptianhieroglyphsScript = 39,
        ElbasanScript = 40,
        ElymaicScript = 41,
        EmojiScript = 42,
        EstrangeloSyriacScript = 43,
        EthiopicScript = 44,
        FrakturLatinScript = 45,
        FraserScript = 46,
        GaelicLatinScript = 47,
        GeorgianKhutsuriScript = 48,
        GeorgianScript = 49,
        GlagoliticScript = 50,
        GothicScript = 51,
        GranthaScript = 52,
        GreekScript = 53,
        GujaratiScript = 54,
        GunjalaGondiScript = 55,
        GurmukhiScript = 56,
        HanScript = 57,
        HangulScript = 58,
        HanifiRohingyaScript = 59,
        HanunooScript = 60,
        HanwithBopomofoScript = 61,
        HatranScript = 62,
        HebrewScript = 63,
        HiraganaScript = 64,
        ImperialAramaicScript = 65,
        IndusScript = 66,
        InheritedScript = 67,
        InscriptionalPahlaviScript = 68,
        InscriptionalParthianScript = 69,
        JamoScript = 70,
        JapaneseScript = 71,
        JapanesesyllabariesScript = 72,
        JavaneseScript = 73,
        JurchenScript = 74,
        KaithiScript = 75,
        KannadaScript = 76,
        KatakanaScript = 77,
        KayahLiScript = 78,
        KharoshthiScript = 79,
        KhitansmallscriptScript = 80,
        KhmerScript = 81,
        KhojkiScript = 82,
        KhudawadiScript = 83,
        KoreanScript = 84,
        KpelleScript = 85,
        LannaScript = 86,
        LaoScript = 87,
        LatinScript = 88,
        LepchaScript = 89,
        LimbuScript = 90,
        LinearAScript = 91,
        LinearBScript = 92,
        LomaScript = 93,
        LycianScript = 94,
        LydianScript = 95,
        MahajaniScript = 96,
        MakasarScript = 97,
        MalayalamScript = 98,
        MandaeanScript = 99,
        ManichaeanScript = 100,
        MarchenScript = 101,
        MasaramGondiScript = 102,
        MathematicalNotationScript = 103,
        MayanhieroglyphsScript = 104,
        MedefaidrinScript = 105,
        MeiteiMayekScript = 106,
        MendeScript = 107,
        MeroiticCursiveScript = 108,
        MeroiticScript = 109,
        ModiScript = 110,
        MongolianScript = 111,
        MoonScript = 112,
        MroScript = 113,
        MultaniScript = 114,
        MyanmarScript = 115,
        NKoScript = 116,
        NabataeanScript = 117,
        NandinagariScript = 118,
        NastaliqScript = 119,
        NaxiGebaScript = 120,
        NewTaiLueScript = 121,
        NewaScript = 122,
        NushuScript = 123,
        NyiakengPuachueHmongScript = 124,
        OdiaScript = 125,
        OghamScript = 126,
        OlChikiScript = 127,
        OldChurchSlavonicCyrillicScript = 128,
        OldHungarianScript = 129,
        OldItalicScript = 130,
        OldNorthArabianScript = 131,
        OldPermicScript = 132,
        OldPersianScript = 133,
        OldSogdianScript = 134,
        OldSouthArabianScript = 135,
        OrkhonScript = 136,
        OsageScript = 137,
        OsmanyaScript = 138,
        PahawhHmongScript = 139,
        PalmyreneScript = 140,
        PauCinHauScript = 141,
        PhagspaScript = 142,
        PhoenicianScript = 143,
        PollardPhoneticScript = 144,
        PsalterPahlaviScript = 145,
        RejangScript = 146,
        RongorongoScript = 147,
        RunicScript = 148,
        SACuneiformScript = 149,
        SamaritanScript = 150,
        SaratiScript = 151,
        SaurashtraScript = 152,
        SharadaScript = 153,
        ShavianScript = 154,
        SiddhamScript = 155,
        SignWritingScript = 156,
        SimplifiedHanScript = 157,
        SinhalaScript = 158,
        SogdianScript = 159,
        SoraSompengScript = 160,
        SoyomboScript = 161,
        SundaneseScript = 162,
        SylotiNagriScript = 163,
        SymbolsScript = 164,
        SyriacScript = 165,
        TagalogScript = 166,
        TagbanwaScript = 167,
        TaiLeScript = 168,
        TaiVietScript = 169,
        TakriScript = 170,
        TamilScript = 171,
        TangutScript = 172,
        TeluguScript = 173,
        TengwarScript = 174,
        ThaanaScript = 175,
        ThaiScript = 176,
        TibetanScript = 177,
        TifinaghScript = 178,
        TirhutaScript = 179,
        TraditionalHanScript = 180,
        UCASScript = 181,
        UgariticScript = 182,
        UnwrittenScript = 183,
        VaiScript = 184,
        VarangKshitiScript = 185,
        VisibleSpeechScript = 186,
        WanchoScript = 187,
        WesternSyriacScript = 188,
        WoleaiScript = 189,
        YezidiScript = 190,
        YiScript = 191,
        ZanabazarSquareScript = 192,
        ZawgyiScript = 193,

        PersoArabicScript = ArabicScript,
        SimplifiedScript = SimplifiedHanScript,
        SumeroAkkadianCuneiformScript = SACuneiformScript,
        TraditionalScript = TraditionalHanScript,
        UnifiedCanadianAboriginalSyllabicsScript = UCASScript,

        LastScript = ZawgyiScript
    };
// GENERATED PART ENDS HERE

    enum MeasurementSystem { MetricSystem, ImperialSystem, UKSystem };

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
    friend class QLocalePrivate;
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
