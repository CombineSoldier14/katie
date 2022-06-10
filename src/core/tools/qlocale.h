/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
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
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QLOCALE_H
#define QLOCALE_H

#include <QtCore/qvariant.h>


QT_BEGIN_NAMESPACE


class QDataStream;
class QDate;
class QDateTime;
class QTime;
class QVariant;
class QTextStream;
class QTextStreamPrivate;

class QLocale;

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
        Atikamekw = 39,
        Atsam = 40,
        AustralianEnglish = 41,
        AustrianGerman = 42,
        Avaric = 43,
        Avestan = 44,
        Awadhi = 45,
        Aymara = 46,
        Azerbaijani = 47,
        Badaga = 48,
        Bafia = 49,
        Bafut = 50,
        Bakhtiari = 51,
        Balinese = 52,
        Baluchi = 53,
        Bambara = 54,
        Bamun = 55,
        Bangla = 56,
        Banjar = 57,
        Basaa = 58,
        Bashkir = 59,
        Basque = 60,
        BatakToba = 61,
        Bavarian = 62,
        Beja = 63,
        Belarusian = 64,
        Bemba = 65,
        Bena = 66,
        Betawi = 67,
        Bhojpuri = 68,
        Bikol = 69,
        Bini = 70,
        Bishnupriya = 71,
        Bislama = 72,
        Blin = 73,
        Blissymbols = 74,
        Bodo = 75,
        Bosnian = 76,
        Brahui = 77,
        Braj = 78,
        BrazilianPortuguese = 79,
        Breton = 80,
        BritishEnglish = 81,
        Buginese = 82,
        Bulgarian = 83,
        Bulu = 84,
        Buriat = 85,
        Burmese = 86,
        Caddo = 87,
        CajunFrench = 88,
        CanadianEnglish = 89,
        CanadianFrench = 90,
        Cantonese = 91,
        Capiznon = 92,
        Carib = 93,
        CarolinaAlgonquian = 94,
        Catalan = 95,
        Cayuga = 96,
        Cebuano = 97,
        CentralAtlasTamazight = 98,
        CentralDusun = 99,
        CentralKurdish = 100,
        CentralOjibwa = 101,
        CentralYupik = 102,
        ChadianArabic = 103,
        Chagatai = 104,
        Chakma = 105,
        Chamorro = 106,
        Chechen = 107,
        Cherokee = 108,
        Cheyenne = 109,
        Chibcha = 110,
        Chickasaw = 111,
        Chiga = 112,
        Chilcotin = 113,
        ChimborazoHighlandQuichua = 114,
        Chinese = 115,
        ChinookJargon = 116,
        Chipewyan = 117,
        Choctaw = 118,
        ChurchSlavic = 119,
        Chuukese = 120,
        Chuvash = 121,
        ClassicalNewari = 122,
        ClassicalSyriac = 123,
        Colognian = 124,
        Comorian = 125,
        CongoSwahili = 126,
        Coptic = 127,
        Cornish = 128,
        Corsican = 129,
        Cree = 130,
        CrimeanTatar = 131,
        Croatian = 132,
        Czech = 133,
        Dakota = 134,
        Danish = 135,
        Dargwa = 136,
        Dari = 137,
        Dazaga = 138,
        Delaware = 139,
        Dinka = 140,
        Divehi = 141,
        Dogri = 142,
        Dogrib = 143,
        Duala = 144,
        Dutch = 145,
        Dyula = 146,
        Dzongkha = 147,
        EasternCanadianInuktitut = 148,
        EasternFrisian = 149,
        EasternOjibwa = 150,
        Efik = 151,
        EgyptianArabic = 152,
        Ekajuk = 153,
        Elamite = 154,
        Embu = 155,
        Emilian = 156,
        English = 157,
        Erzya = 158,
        Esperanto = 159,
        Estonian = 160,
        EuropeanPortuguese = 161,
        EuropeanSpanish = 162,
        Ewe = 163,
        Ewondo = 164,
        Extremaduran = 165,
        Fang = 166,
        Fanti = 167,
        Faroese = 168,
        FijiHindi = 169,
        Fijian = 170,
        Filipino = 171,
        Finnish = 172,
        Flemish = 173,
        Fon = 174,
        Frafra = 175,
        French = 176,
        Friulian = 177,
        Fulah = 178,
        Ga = 179,
        Gagauz = 180,
        Galician = 181,
        GanChinese = 182,
        Ganda = 183,
        Gayo = 184,
        Gbaya = 185,
        Geez = 186,
        Georgian = 187,
        German = 188,
        GhegAlbanian = 189,
        Ghomala = 190,
        Gilaki = 191,
        Gilbertese = 192,
        GoanKonkani = 193,
        Gondi = 194,
        Gorontalo = 195,
        Gothic = 196,
        Grebo = 197,
        Greek = 198,
        Guarani = 199,
        Gujarati = 200,
        Gusii = 201,
        Gwichin = 202,
        Haida = 203,
        HaitianCreole = 204,
        HakkaChinese = 205,
        Halkomelem = 206,
        Hausa = 207,
        Hawaiian = 208,
        Hebrew = 209,
        Herero = 210,
        Hiligaynon = 211,
        Hindi = 212,
        HiriMotu = 213,
        Hittite = 214,
        Hmong = 215,
        HmongNjua = 216,
        Hungarian = 217,
        Hupa = 218,
        Iban = 219,
        Ibibio = 220,
        Icelandic = 221,
        Ido = 222,
        Igbo = 223,
        Iloko = 224,
        InariSami = 225,
        Indonesian = 226,
        Ingrian = 227,
        Ingush = 228,
        Innuaimun = 229,
        Interlingua = 230,
        Interlingue = 231,
        Inuktitut = 232,
        Inupiaq = 233,
        Irish = 234,
        Italian = 235,
        JamaicanCreoleEnglish = 236,
        Japanese = 237,
        Javanese = 238,
        Jju = 239,
        JolaFonyi = 240,
        JudeoArabic = 241,
        JudeoPersian = 242,
        Jutish = 243,
        Kabardian = 244,
        Kabuverdianu = 245,
        Kabyle = 246,
        Kachin = 247,
        Kaingang = 248,
        Kako = 249,
        Kalaallisut = 250,
        Kalenjin = 251,
        Kalmyk = 252,
        Kamba = 253,
        Kanembu = 254,
        Kannada = 255,
        Kanuri = 256,
        KaraKalpak = 257,
        KarachayBalkar = 258,
        Karelian = 259,
        Kashmiri = 260,
        Kashubian = 261,
        Kawi = 262,
        Kazakh = 263,
        Kenyang = 264,
        Khasi = 265,
        Khmer = 266,
        Khotanese = 267,
        Khowar = 268,
        Kiche = 269,
        Kikuyu = 270,
        Kimbundu = 271,
        Kinaraya = 272,
        Kinyarwanda = 273,
        Kirghiz = 274,
        Kirmanjki = 275,
        Klingon = 276,
        Kom = 277,
        Komi = 278,
        KomiPermyak = 279,
        Kongo = 280,
        Konkani = 281,
        Korean = 282,
        Koro = 283,
        Kosraean = 284,
        Kotava = 285,
        KoyraChiini = 286,
        KoyraboroSenni = 287,
        Kpelle = 288,
        Krio = 289,
        Kuanyama = 290,
        Kumyk = 291,
        Kurdish = 292,
        Kurukh = 293,
        Kutenai = 294,
        Kwakwala = 295,
        Kwasio = 296,
        Ladino = 297,
        Lahnda = 298,
        Lakota = 299,
        Lamba = 300,
        Langi = 301,
        Lao = 302,
        Latgalian = 303,
        Latin = 304,
        LatinAmericanSpanish = 305,
        Latvian = 306,
        Laz = 307,
        Lezghian = 308,
        Ligurian = 309,
        Lillooet = 310,
        Limburgish = 311,
        Lingala = 312,
        LinguaFrancaNova = 313,
        LiteraryChinese = 314,
        Lithuanian = 315,
        Livonian = 316,
        Lojban = 317,
        Lombard = 318,
        LouisianaCreole = 319,
        LowGerman = 320,
        LowSaxon = 321,
        LowerSilesian = 322,
        LowerSorbian = 323,
        Lozi = 324,
        LubaKatanga = 325,
        LubaLulua = 326,
        Luiseno = 327,
        LuleSami = 328,
        Lunda = 329,
        Luo = 330,
        Luxembourgish = 331,
        Luyia = 332,
        Maba = 333,
        Macedonian = 334,
        Machame = 335,
        Madurese = 336,
        Mafa = 337,
        Magahi = 338,
        MainFranconian = 339,
        Maithili = 340,
        Makasar = 341,
        MakhuwaMeetto = 342,
        Makonde = 343,
        Malagasy = 344,
        Malay = 345,
        Malayalam = 346,
        Malecite = 347,
        Maltese = 348,
        Manchu = 349,
        Mandar = 350,
        Mandingo = 351,
        Manipuri = 352,
        Manx = 353,
        Maori = 354,
        Mapuche = 355,
        Marathi = 356,
        Mari = 357,
        Marshallese = 358,
        Marwari = 359,
        Masai = 360,
        Mazanderani = 361,
        Medumba = 362,
        Mende = 363,
        Mentawai = 364,
        Meru = 365,
        Meta = 366,
        MexicanSpanish = 367,
        Michif = 368,
        MiddleDutch = 369,
        MiddleEnglish = 370,
        MiddleFrench = 371,
        MiddleHighGerman = 372,
        MiddleIrish = 373,
        Mikmaq = 374,
        MinNanChinese = 375,
        Minangkabau = 376,
        Mingrelian = 377,
        Mirandese = 378,
        Mizo = 379,
        ModernStandardArabic = 380,
        Mohawk = 381,
        Moksha = 382,
        Moldavian = 383,
        Mongo = 384,
        Mongolian = 385,
        Montenegrin = 386,
        MooseCree = 387,
        Morisyen = 388,
        MoroccanArabic = 389,
        Mossi = 390,
        Multiplelanguages = 391,
        Mundang = 392,
        Muscogee = 393,
        MuslimTat = 394,
        Myene = 395,
        NKo = 396,
        Nama = 397,
        NauruLanguage = 398,
        Navajo = 399,
        Ndonga = 400,
        Neapolitan = 401,
        Nepali = 402,
        Newari = 403,
        Ngambay = 404,
        Ngiemboon = 405,
        Ngomba = 406,
        Nheengatu = 407,
        Nias = 408,
        NigerianPidgin = 409,
        Niuean = 410,
        Nogai = 411,
        Nolinguisticcontent = 412,
        NorthNdebele = 413,
        NorthernEastCree = 414,
        NorthernFrisian = 415,
        NorthernHaida = 416,
        NorthernLuri = 417,
        NorthernSami = 418,
        NorthernSotho = 419,
        NorthernTutchone = 420,
        NorthwesternOjibwa = 421,
        Norwegian = 422,
        NorwegianBokmal = 423,
        NorwegianNynorsk = 424,
        Novial = 425,
        Nuer = 426,
        Nyamwezi = 427,
        Nyanja = 428,
        Nyankole = 429,
        NyasaTonga = 430,
        Nyoro = 431,
        Nzima = 432,
        Occitan = 433,
        Odia = 434,
        OjiCree = 435,
        Ojibwa = 436,
        Okanagan = 437,
        OldEnglish = 438,
        OldFrench = 439,
        OldHighGerman = 440,
        OldIrish = 441,
        OldNorse = 442,
        OldPersian = 443,
        OldProvencal = 444,
        Oromo = 445,
        Osage = 446,
        Ossetic = 447,
        OttomanTurkish = 448,
        Pahlavi = 449,
        PalatineGerman = 450,
        Palauan = 451,
        Pali = 452,
        Pampanga = 453,
        Pangasinan = 454,
        Papiamento = 455,
        Pashto = 456,
        PennsylvaniaGerman = 457,
        Persian = 458,
        Phoenician = 459,
        Picard = 460,
        Piedmontese = 461,
        PlainsCree = 462,
        Plautdietsch = 463,
        Pohnpeian = 464,
        Polish = 465,
        Pontic = 466,
        Portuguese = 467,
        Prussian = 468,
        Punjabi = 469,
        Quechua = 470,
        Rajasthani = 471,
        Rapanui = 472,
        Rarotongan = 473,
        Riffian = 474,
        Rohingya = 475,
        Romagnol = 476,
        Romanian = 477,
        Romansh = 478,
        Romany = 479,
        Rombo = 480,
        Rotuman = 481,
        Roviana = 482,
        Rundi = 483,
        Russian = 484,
        Rusyn = 485,
        Rwa = 486,
        Saho = 487,
        Sakha = 488,
        SamaritanAramaic = 489,
        Samburu = 490,
        SamiSkolt = 491,
        SamiSouthern = 492,
        Samoan = 493,
        Samogitian = 494,
        Sandawe = 495,
        Sango = 496,
        Sangu = 497,
        Sanskrit = 498,
        Santali = 499,
        Sardinian = 500,
        Sasak = 501,
        SassareseSardinian = 502,
        SaterlandFrisian = 503,
        Saurashtra = 504,
        Scots = 505,
        ScottishGaelic = 506,
        Selayar = 507,
        Selkup = 508,
        Sena = 509,
        Seneca = 510,
        Serbian = 511,
        SerboCroatian = 512,
        Serer = 513,
        Seri = 514,
        SeselwaCreoleFrench = 515,
        Shambala = 516,
        Shan = 517,
        Shona = 518,
        SichuanYi = 519,
        Sicilian = 520,
        Sidamo = 521,
        Siksika = 522,
        Silesian = 523,
        SimplifiedChinese = 524,
        Sindhi = 525,
        Sinhala = 526,
        Slave = 527,
        Slovak = 528,
        Slovenian = 529,
        Soga = 530,
        Sogdien = 531,
        Somali = 532,
        Soninke = 533,
        SouthNdebele = 534,
        SouthernAltai = 535,
        SouthernEastCree = 536,
        SouthernHaida = 537,
        SouthernKurdish = 538,
        SouthernLushootseed = 539,
        SouthernSotho = 540,
        SouthernTutchone = 541,
        Spanish = 542,
        SrananTongo = 543,
        StandardMoroccanTamazight = 544,
        StraitsSalish = 545,
        Sukuma = 546,
        Sumerian = 547,
        Sundanese = 548,
        Susu = 549,
        Swahili = 550,
        SwampyCree = 551,
        Swati = 552,
        Swedish = 553,
        SwissFrench = 554,
        SwissGerman = 555,
        SwissHighGerman = 556,
        Syriac = 557,
        Tachelhit = 558,
        Tagalog = 559,
        Tagish = 560,
        Tahitian = 561,
        Tahltan = 562,
        TaiDam = 563,
        Taita = 564,
        Tajik = 565,
        Talysh = 566,
        Tamashek = 567,
        Tamil = 568,
        Taroko = 569,
        Tasawaq = 570,
        Tatar = 571,
        Telugu = 572,
        Tereno = 573,
        Teso = 574,
        Tetum = 575,
        Thai = 576,
        Tibetan = 577,
        Tigre = 578,
        Tigrinya = 579,
        Timne = 580,
        Tiv = 581,
        Tlingit = 582,
        TokPisin = 583,
        TokelauLanguage = 584,
        Tongan = 585,
        TornedalenFinnish = 586,
        Torwali = 587,
        TraditionalChinese = 588,
        Tsakhur = 589,
        Tsakonian = 590,
        Tsimshian = 591,
        Tsonga = 592,
        Tswana = 593,
        Tulu = 594,
        Tumbuka = 595,
        TunisianArabic = 596,
        Turkish = 597,
        Turkmen = 598,
        Turoyo = 599,
        TuvaluLanguage = 600,
        Tuvinian = 601,
        Twi = 602,
        Tyap = 603,
        Udmurt = 604,
        Ugaritic = 605,
        Uighur = 606,
        Ukrainian = 607,
        Umbundu = 608,
        Unknownlanguage = 609,
        UpperSorbian = 610,
        Urdu = 611,
        Uzbek = 612,
        Vai = 613,
        Venda = 614,
        Venetian = 615,
        Veps = 616,
        Vietnamese = 617,
        Volapuk = 618,
        Voro = 619,
        Votic = 620,
        Vunjo = 621,
        Walloon = 622,
        Walser = 623,
        Waray = 624,
        Warlpiri = 625,
        Washo = 626,
        Wayuu = 627,
        Welsh = 628,
        WestFlemish = 629,
        WesternBalochi = 630,
        WesternCanadianInuktitut = 631,
        WesternFrisian = 632,
        WesternMari = 633,
        WesternOjibwa = 634,
        Wolaytta = 635,
        Wolof = 636,
        WoodsCree = 637,
        WuChinese = 638,
        Xhosa = 639,
        XiangChinese = 640,
        Yangben = 641,
        Yao = 642,
        Yapese = 643,
        Yemba = 644,
        Yiddish = 645,
        Yoruba = 646,
        Zapotec = 647,
        Zarma = 648,
        Zaza = 649,
        Zeelandic = 650,
        Zenaga = 651,
        Zhuang = 652,
        ZoroastrianDari = 653,
        Zulu = 654,
        Zuni = 655,

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
        SamiInari = InariSami,
        SamiLule = LuleSami,
        SamiNorthern = NorthernSami,
        SimplifiedMandarinChinese = SimplifiedChinese,
        SkoltSami = SamiSkolt,
        SouthernSami = SamiSouthern,
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
        Yemen = 291,
        Zambia = 292,
        Zimbabwe = 293,
        world = 294,

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

        LastCountry = world
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
        CyproMinoanScript = 30,
        CyrillicScript = 31,
        DeseretScript = 32,
        DevanagariScript = 33,
        DivesAkuruScript = 34,
        DograScript = 35,
        DuployanshorthandScript = 36,
        EasternSyriacScript = 37,
        EgyptiandemoticScript = 38,
        EgyptianhieraticScript = 39,
        EgyptianhieroglyphsScript = 40,
        ElbasanScript = 41,
        ElymaicScript = 42,
        EmojiScript = 43,
        EstrangeloSyriacScript = 44,
        EthiopicScript = 45,
        FrakturLatinScript = 46,
        FraserScript = 47,
        GaelicLatinScript = 48,
        GeorgianKhutsuriScript = 49,
        GeorgianScript = 50,
        GlagoliticScript = 51,
        GothicScript = 52,
        GranthaScript = 53,
        GreekScript = 54,
        GujaratiScript = 55,
        GunjalaGondiScript = 56,
        GurmukhiScript = 57,
        HanScript = 58,
        HangulScript = 59,
        HanifiRohingyaScript = 60,
        HanunooScript = 61,
        HanwithBopomofoScript = 62,
        HatranScript = 63,
        HebrewScript = 64,
        HiraganaScript = 65,
        ImperialAramaicScript = 66,
        IndusScript = 67,
        InheritedScript = 68,
        InscriptionalPahlaviScript = 69,
        InscriptionalParthianScript = 70,
        JamoScript = 71,
        JapaneseScript = 72,
        JapanesesyllabariesScript = 73,
        JavaneseScript = 74,
        JurchenScript = 75,
        KaithiScript = 76,
        KannadaScript = 77,
        KatakanaScript = 78,
        KawiScript = 79,
        KayahLiScript = 80,
        KharoshthiScript = 81,
        KhitansmallscriptScript = 82,
        KhmerScript = 83,
        KhojkiScript = 84,
        KhudawadiScript = 85,
        KoreanScript = 86,
        KpelleScript = 87,
        LannaScript = 88,
        LaoScript = 89,
        LatinScript = 90,
        LepchaScript = 91,
        LimbuScript = 92,
        LinearAScript = 93,
        LinearBScript = 94,
        LomaScript = 95,
        LycianScript = 96,
        LydianScript = 97,
        MahajaniScript = 98,
        MakasarScript = 99,
        MalayalamScript = 100,
        MandaeanScript = 101,
        ManichaeanScript = 102,
        MarchenScript = 103,
        MasaramGondiScript = 104,
        MathematicalNotationScript = 105,
        MayanhieroglyphsScript = 106,
        MedefaidrinScript = 107,
        MeiteiMayekScript = 108,
        MendeScript = 109,
        MeroiticCursiveScript = 110,
        MeroiticScript = 111,
        ModiScript = 112,
        MongolianScript = 113,
        MoonScript = 114,
        MroScript = 115,
        MultaniScript = 116,
        MyanmarScript = 117,
        NKoScript = 118,
        NabataeanScript = 119,
        NagMundariScript = 120,
        NandinagariScript = 121,
        NastaliqScript = 122,
        NaxiGebaScript = 123,
        NewTaiLueScript = 124,
        NewaScript = 125,
        NushuScript = 126,
        NyiakengPuachueHmongScript = 127,
        OdiaScript = 128,
        OghamScript = 129,
        OlChikiScript = 130,
        OldChurchSlavonicCyrillicScript = 131,
        OldHungarianScript = 132,
        OldItalicScript = 133,
        OldNorthArabianScript = 134,
        OldPermicScript = 135,
        OldPersianScript = 136,
        OldSogdianScript = 137,
        OldSouthArabianScript = 138,
        OldUyghurScript = 139,
        OrkhonScript = 140,
        OsageScript = 141,
        OsmanyaScript = 142,
        PahawhHmongScript = 143,
        PalmyreneScript = 144,
        PauCinHauScript = 145,
        PhagspaScript = 146,
        PhoenicianScript = 147,
        PollardPhoneticScript = 148,
        PsalterPahlaviScript = 149,
        RejangScript = 150,
        RongorongoScript = 151,
        RunicScript = 152,
        SACuneiformScript = 153,
        SamaritanScript = 154,
        SaratiScript = 155,
        SaurashtraScript = 156,
        SharadaScript = 157,
        ShavianScript = 158,
        SiddhamScript = 159,
        SignWritingScript = 160,
        SimplifiedHanScript = 161,
        SinhalaScript = 162,
        SogdianScript = 163,
        SoraSompengScript = 164,
        SoyomboScript = 165,
        SundaneseScript = 166,
        SylotiNagriScript = 167,
        SymbolsScript = 168,
        SyriacScript = 169,
        TagalogScript = 170,
        TagbanwaScript = 171,
        TaiLeScript = 172,
        TaiVietScript = 173,
        TakriScript = 174,
        TamilScript = 175,
        TangsaScript = 176,
        TangutScript = 177,
        TeluguScript = 178,
        TengwarScript = 179,
        ThaanaScript = 180,
        ThaiScript = 181,
        TibetanScript = 182,
        TifinaghScript = 183,
        TirhutaScript = 184,
        TotoScript = 185,
        TraditionalHanScript = 186,
        UCASScript = 187,
        UgariticScript = 188,
        UnwrittenScript = 189,
        VaiScript = 190,
        VarangKshitiScript = 191,
        VisibleSpeechScript = 192,
        VithkuqiScript = 193,
        WanchoScript = 194,
        WesternSyriacScript = 195,
        WoleaiScript = 196,
        YezidiScript = 197,
        YiScript = 198,
        ZanabazarSquareScript = 199,
        ZawgyiScript = 200,

        HanifiScript = HanifiRohingyaScript,
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

    QByteArray bcp47Name() const;
    QString nativeLanguageName() const;
    QString nativeCountryName() const;

    short toShort(const QString &s, bool *ok = nullptr, int base = 0) const;
    ushort toUShort(const QString &s, bool *ok = nullptr, int base = 0) const;
    int toInt(const QString &s, bool *ok = nullptr, int base = 0) const;
    uint toUInt(const QString &s, bool *ok = nullptr, int base = 0) const;
    qlonglong toLongLong(const QString &s, bool *ok = nullptr, int base = 0) const;
    qulonglong toULongLong(const QString &s, bool *ok = nullptr, int base = 0) const;
    float toFloat(const QString &s, bool *ok = nullptr) const;
    double toDouble(const QString &s, bool *ok = nullptr) const;

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

#ifndef QT_NO_DATASTREAM
Q_CORE_EXPORT QDataStream &operator<<(QDataStream &, const QLocale &);
Q_CORE_EXPORT QDataStream &operator>>(QDataStream &, QLocale &);
#endif

QT_END_NAMESPACE

#endif // QLOCALE_H
