//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/08/05.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#ifndef ILLYBLAUNCHER_FACEREVENTKEY_HXX
#define ILLYBLAUNCHER_FACEREVENTKEY_HXX

namespace Facer {
    
    // you'd better disable some warning flags about
    //  'unused' for your compiler/linter/IDE such as
    //  the 'unused global declaration' of JetBrains CLion.
    enum InputKeyCode {
        KNull,
        Kx01,
        Kx02,
        Kx03,
        Kx04,
        Kx05,
        Kx06,
        Kx07,
        KBackspace = 0x08,
        KTab = 0x09,
        KLF = 0x0A,
        Kx0B = 0x0B,
        Kx0C = 0x0C,
        KReturn = 0x0D,
        Kx0E = 0x0E,
        Kx0F = 0x0F,
        KShift = 0x10, // KeyboardEvent.keyCode
        KControl = 0x11, // and Windows Virtual-Key Codes
        KOption = 0x12,
        
        KSuperLeft = 0x5B,
        KSuperRight = 0x5C,
        
        Kx13 = 0x13,
        Kx14 = 0x14,
        Kx15 = 0x15,
        Kx16 = 0x16,
        Kx17 = 0x17,
        Kx18 = 0x18,
        Kx19 = 0x19,
        Kx2A = 0x1A,
        KEscape = 0x1B,
        Kx1C = 0x1C,
        Kx1D = 0x1D,
        Kx1E = 0x1E,
        Kx1F = 0x1F,
        KSpace = 0x20,
        KExclamation = 0x21,
        KQuotesDouble = 0x22,
        KNumber = 0x23,
        KDollar = 0x24,
        KPercent = 0x25,
        KAmpersand = 0x26,
        KQuoteSingle = 0x27,
        KParenthesisLeft = 0x28,
        KParenthesisRight = 0x29,
        KAsterisk = 0x2A,
        KPlus = 0x2B,
        KComma = 0x2C,
        KHyphen = 0x2D,
        KPeriod = 0x2E,
        KSlash = 0x2F,
        
        KNumber0 = 0x30,
        KNumber1 = 0x31,
        KNumber2 = 0x32,
        KNumber3 = 0x33,
        KNumber4 = 0x34,
        KNumber5 = 0x35,
        KNumber6 = 0x36,
        KNumber7 = 0x37,
        KNumber8 = 0x38,
        KNumber9 = 0x39,
        
        KColon = 0x3A,
        KSemicolon = 0x3B,
        KLessThan = 0x3C,
        KEqual = 0x3D,
        KGreaterThan = 0x3E,
        KQuestion = 0x3F,
        KAt = 0x40,
        
        KLetterCapA = 0x41,
        KLetterCapB = 0x42,
        KLetterCapC = 0x43,
        KLetterCapD = 0x44,
        KLetterCapE = 0x45,
        KLetterCapF = 0x46,
        KLetterCapG = 0x47,
        KLetterCapH = 0x48,
        KLetterCapI = 0x49,
        KLetterCapJ = 0x4A,
        KLetterCapK = 0x4B,
        KLetterCapL = 0x4C,
        KLetterCapM = 0x4D,
        KLetterCapN = 0x4E,
        KLetterCapO = 0x4F,
        KLetterCapP = 0x50,
        KLetterCapQ = 0x51,
        KLetterCapR = 0x52,
        KLetterCapS = 0x53,
        KLetterCapT = 0x54,
        KLetterCapU = 0x55,
        KLetterCapV = 0x56,
        KLetterCapW = 0x57,
        KLetterCapX = 0x58,
        KLetterCapY = 0x59,
        KLetterCapZ = 0x5A,
        
        KBracketLeft = 0x5B,
        KBackslash = 0x5C,
        KBracketRight = 0x5D,
        KCaret = 0x5E,
        KUnderscore = 0x5F,
        KGraveAccent = 0x60,
        
        KLetterA = 0x61,
        KLetterB = 0x62,
        KLetterC = 0x63,
        KLetterD = 0x64,
        KLetterE = 0x65,
        KLetterF = 0x66,
        KLetterG = 0x67,
        KLetterH = 0x68,
        KLetterI = 0x69,
        KLetterJ = 0x6A,
        KLetterK = 0x6B,
        KLetterL = 0x6C,
        KLetterM = 0x6D,
        KLetterN = 0x6E,
        KLetterO = 0x6F,
        KLetterP = 0x70,
        KLetterQ = 0x71,
        KLetterR = 0x72,
        KLetterS = 0x73,
        KLetterT = 0x74,
        KLetterU = 0x75,
        KLetterV = 0x76,
        KLetterW = 0x77,
        KLetterX = 0x78,
        KLetterY = 0x79,
        KLetterZ = 0x7A,
        
        KBraceLeft = 0x7B,
        KBar = 0x7C,
        KBraceRight = 0x7D,
        KTilde = 0x7E,
        KDelete = 0x7F,
        
    };
    
}

#endif // ILLYBLAUNCHER_FACEREVENTKEY_HXX
