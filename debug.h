#ifndef DEBUG_H
#define DEBUG_H

#include<QDebug>

#define D_CONDITION 0
#define D if(D_CONDITION)

#define D_DATABASE_CONDITION (0 && D_CONDITION)
#define D_DATABASE if(D_DATABASE_CONDITION)

#define D_THRED_CONDITION (0 && D_CONDITION)
#define D_THRED if(D_THRED_CONDITION)

#define D_JSON_CONDITION (0 && D_CONDITION)
#define D_JSON if(D_JSON_CONDITION)

#define D_MAINWINDOW_CONDITION (0 && D_CONDITION)
#define D_MAINWINDOW if(D_MAINWINDOW_CONDITION)

#define D_NEWPAGEMODEL_CONDITION (0 && D_CONDITION)
#define D_NEWPAGEMODEL if(D_NEWPAGEMODEL_CONDITION)

#define D_PAGECOMPARATOR_CONDITION (0 && D_CONDITION)
#define D_PAGECOMPARATOR if(D_PAGECOMPARATOR_CONDITION)

#define D_PAGEVIEWER_CONDITION (0 && D_CONDITION)
#define D_PAGEVIEWER if(D_PAGEVIEWER_CONDITION)
#endif // DEBUG_H
