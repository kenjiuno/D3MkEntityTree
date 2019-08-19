
#include "StdAfx.h"
#include "ToolContext.h"
#include "PM.h"

// ->
//  ->
//   ->
//    ->
// CToolContext
//      ->
//       ->
//        ->
//         ->

void CToolContext::SetDoc(CDocument *pDoc, bool fSet)
{
	if (!fSet) {
		if (pDocSet != NULL) {
			Dispatch(PM_SET_DOC, NULL);
			pDocSet = NULL;
		}
		return;
	}
	if (pDocSet != pDoc) {
		Dispatch(PM_SET_DOC, pDoc);
		pDocSet = pDoc;
	}
}
