/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef FICSCLIENT_H
#define FICSCLIENT_H

#include "telnetclient.h"

class FicsClient : public TelnetClient
{
    Q_OBJECT

public:
    FicsClient(QObject* parent);

    enum ClientCode
    {
        StartReply = 21,
        Separator  = 22,
        EndReply   = 23,
        PoseStart  = 24,
        PoseEnd    = 25
    };

    enum CommandCode
    {
        BLKCMD_NULL = 0,
        BLKCMD_GAME_MOVE = 1,
        BLKCMD_ABORT = 10   ,
        BLKCMD_ACCEPT = 11  ,
        BLKCMD_ADDLIST = 12 ,
        BLKCMD_ADJOURN = 13 ,
        BLKCMD_ALLOBSERVERS = 14,
        BLKCMD_ASSESS = 15   ,
        BLKCMD_BACKWARD = 16 ,
        BLKCMD_BELL = 17     ,
        BLKCMD_BEST = 18     ,
        BLKCMD_BNAME = 19    ,
        BLKCMD_BOARDS = 20   ,
        BLKCMD_BSETUP = 21        ,
        BLKCMD_BUGWHO = 22        ,
        BLKCMD_CBEST = 23         ,
        BLKCMD_CLEARMESSAGES = 24 ,
        BLKCMD_CLRSQUARE = 25     ,
        BLKCMD_CONVERT_BCF = 26   ,
        BLKCMD_CONVERT_ELO = 27  ,
        BLKCMD_CONVERT_USCF = 28 ,
        BLKCMD_COPYGAME = 29     ,
        BLKCMD_CRANK = 30        ,
        BLKCMD_CSHOUT = 31       ,
        BLKCMD_DATE = 32         ,
        BLKCMD_DECLINE = 33  ,
        BLKCMD_DRAW = 34     ,
        BLKCMD_ECO = 35      ,
        BLKCMD_EXAMINE = 36  ,
        BLKCMD_FINGER = 37   ,
        BLKCMD_FLAG = 38     ,
        BLKCMD_FLIP = 39    ,
        BLKCMD_FMESSAGE = 40,
        BLKCMD_FOLLOW = 41  ,
        BLKCMD_FORWARD = 42 ,
        BLKCMD_GAMES = 43   ,
        BLKCMD_GETGI = 44   ,
        BLKCMD_GETPI = 45    ,
        BLKCMD_GINFO = 46    ,
        BLKCMD_GOBOARD = 47  ,
        BLKCMD_HANDLES = 48  ,
        BLKCMD_HBEST = 49    ,
        BLKCMD_HELP = 50     ,
        BLKCMD_HISTORY = 51    ,
        BLKCMD_HRANK = 52      ,
        BLKCMD_INCHANNEL = 53  ,
        BLKCMD_INDEX = 54      ,
        BLKCMD_INFO = 55       ,
        BLKCMD_ISET = 56       ,
        BLKCMD_IT = 57          ,
        BLKCMD_IVARIABLES = 58  ,
        BLKCMD_JKILL = 59       ,
        BLKCMD_JOURNAL = 60     ,
        BLKCMD_JSAVE = 61       ,
        BLKCMD_KIBITZ = 62      ,
        BLKCMD_LIMITS = 63        ,
        BLKCMD_LLOGONS = 65       ,
        BLKCMD_LOGONS = 66        ,
        BLKCMD_MAILHELP = 67      ,
        BLKCMD_MAILMESS = 68      ,
        BLKCMD_MAILMOVES = 69     ,
        BLKCMD_MAILOLDMOVES = 70,
        BLKCMD_MAILSOURCE = 71  ,
        BLKCMD_MAILSTORED = 72  ,
        BLKCMD_MATCH = 73       ,
        BLKCMD_MESSAGES = 74    ,
        BLKCMD_MEXAMINE = 75    ,
        BLKCMD_MORETIME = 76,
        BLKCMD_MOVES = 77   ,
        BLKCMD_NEWS = 78    ,
        BLKCMD_NEXT = 79    ,
        BLKCMD_OBSERVE = 80 ,
        BLKCMD_OLDMOVES = 81,
        BLKCMD_OLDSTORED = 82,
        BLKCMD_OPEN = 83     ,
        BLKCMD_PARTNER = 84  ,
        BLKCMD_PASSWORD = 85 ,
        BLKCMD_PAUSE = 86    ,
        BLKCMD_PENDING = 87  ,
        BLKCMD_PFOLLOW = 88 ,
        BLKCMD_POBSERVE = 89,
        BLKCMD_PREFRESH = 90,
        BLKCMD_PRIMARY = 91 ,
        BLKCMD_PROMOTE = 92 ,
        BLKCMD_PSTAT = 93   ,
        BLKCMD_PTELL = 94     ,
        BLKCMD_PTIME = 95     ,
        BLKCMD_QTELL = 96     ,
        BLKCMD_QUIT = 97      ,
        BLKCMD_RANK = 98      ,
        BLKCMD_RCOPYGAME = 99 ,
        BLKCMD_RFOLLOW = 100 ,
        BLKCMD_REFRESH = 101 ,
        BLKCMD_REMATCH = 102 ,
        BLKCMD_RESIGN = 103  ,
        BLKCMD_RESUME = 104  ,
        BLKCMD_REVERT = 105  ,
        BLKCMD_ROBSERVE = 106 ,
        BLKCMD_SAY = 107      ,
        BLKCMD_SERVERS = 108  ,
        BLKCMD_SET = 109      ,
        BLKCMD_SHOUT = 110    ,
        BLKCMD_SHOWLIST = 111 ,
        BLKCMD_SIMABORT = 112     ,
        BLKCMD_SIMALLABORT = 113  ,
        BLKCMD_SIMADJOURN = 114   ,
        BLKCMD_SIMALLADJOURN = 115,
        BLKCMD_SIMGAMES = 116     ,
        BLKCMD_SIMMATCH = 117     ,
        BLKCMD_SIMNEXT = 118    ,
        BLKCMD_SIMOBSERVE = 119 ,
        BLKCMD_SIMOPEN = 120    ,
        BLKCMD_SIMPASS = 121    ,
        BLKCMD_SIMPREV = 122    ,
        BLKCMD_SMOVES = 123     ,
        BLKCMD_SMPOSITION = 124 ,
        BLKCMD_SPOSITION = 125  ,
        BLKCMD_STATISTICS = 126 ,
        BLKCMD_STORED = 127     ,
        BLKCMD_STYLE = 128      ,
        BLKCMD_SWITCH = 130     ,
        BLKCMD_TAKEBACK = 131 ,
        BLKCMD_TELL = 132     ,
        BLKCMD_TIME = 133     ,
        BLKCMD_TOMOVE = 134   ,
        BLKCMD_TOURNSET = 135 ,
        BLKCMD_UNALIAS = 136  ,
        BLKCMD_UNEXAMINE = 137,
        BLKCMD_UNOBSERVE = 138,
        BLKCMD_UNPAUSE = 139  ,
        BLKCMD_UPTIME = 140   ,
        BLKCMD_USCF = 141     ,
        BLKCMD_USTAT = 142    ,
        BLKCMD_VARIABLES = 143,
        BLKCMD_WHENSHUT = 144 ,
        BLKCMD_WHISPER = 145  ,
        BLKCMD_WHO = 146      ,
        BLKCMD_WITHDRAW = 147 ,
        BLKCMD_WNAME = 148    ,
        BLKCMD_XKIBITZ = 149  ,
        BLKCMD_XTELL = 150    ,
        BLKCMD_XWHISPER = 151 ,
        BLKCMD_ZNOTIFY = 152  ,
        BLKCMD_SUMMON = 154   ,
        BLKCMD_SEEK = 155     ,
        BLKCMD_UNSEEK = 156  ,
        BLKCMD_SOUGHT = 157  ,
        BLKCMD_PLAY = 158    ,
        BLKCMD_ALIAS = 159   ,
        BLKCMD_NEWBIES = 160 ,
        BLKCMD_SR = 161      ,
        BLKCMD_CA = 162     ,
        BLKCMD_TM = 163     ,
        BLKCMD_GETGAME = 164,
        BLKCMD_CCNEWSE = 165,
        BLKCMD_CCNEWSF = 166,
        BLKCMD_CCNEWSI = 167,
        BLKCMD_CCNEWSP = 168,
        BLKCMD_CCNEWST = 169,
        BLKCMD_CSNEWSE = 170,
        BLKCMD_CSNEWSF = 171,
        BLKCMD_CSNEWSI = 172,
        BLKCMD_CSNEWSP = 173,
        BLKCMD_CSNEWST = 174,
        BLKCMD_CTNEWSE = 175,
        BLKCMD_CTNEWSF = 176,
        BLKCMD_CTNEWSI = 177,
        BLKCMD_CTNEWSP = 178,
        BLKCMD_CTNEWST = 179,
        BLKCMD_CNEWS = 180,
        BLKCMD_SNEWS = 181,
        BLKCMD_TNEWS = 182,
        BLKCMD_RMATCH = 183,
        BLKCMD_RSTAT = 184,
        BLKCMD_CRSTAT = 185,
        BLKCMD_HRSTAT = 186,
        BLKCMD_GSTAT = 187,

        BLKCMD_ERROR_BADCOMMAND = 512,
        BLKCMD_ERROR_BADPARAMS = 513,
        BLKCMD_ERROR_AMBIGUOUS = 514,
        BLKCMD_ERROR_RIGHTS = 515,
        BLKCMD_ERROR_OBSOLETE = 516,
        BLKCMD_ERROR_REMOVED = 517,
        BLKCMD_ERROR_NOTPLAYING = 518,
        BLKCMD_ERROR_NOSEQUENCE = 519,
        BLKCMD_ERROR_LENGTH = 520
    };

    virtual void startSession();
    virtual void sendAccept();
    virtual void sendHistory();
    virtual void sendPlayRequest(int gameId);
    virtual void sendCommand(QString s);
    virtual void sendObserve(int gameId);
    virtual void sendUnobserve(int gameId);

signals:
    void addNewGameEntry(QString s);
    void addNewHistoryEntry(QString s);

protected:
    void sendFicsCommand(QString s);
    void sendFicsCommandWithId(QString s, int id);
    virtual void OnSessionStarted();
    virtual void OnReceiveTelnetMessage(QString);

};

#endif // FICSCLIENT_H
