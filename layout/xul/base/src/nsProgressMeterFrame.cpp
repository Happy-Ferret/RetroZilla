/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is Mozilla Communicator client code.
 *
 * The Initial Developer of the Original Code is
 * Netscape Communications Corporation.
 * Portions created by the Initial Developer are Copyright (C) 1998
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either of the GNU General Public License Version 2 or later (the "GPL"),
 * or the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

//
// David Hyatt & Eric Vaughan
// Netscape Communications
//
// See documentation in associated header file
//

#include "nsProgressMeterFrame.h"
#include "nsCSSRendering.h"
#include "nsIContent.h"
#include "nsPresContext.h"
#include "nsHTMLAtoms.h"
#include "nsXULAtoms.h"
#include "nsINameSpaceManager.h"
#include "nsCOMPtr.h"
#include "nsBoxLayoutState.h"
//
// NS_NewToolbarFrame
//
// Creates a new Toolbar frame and returns it in |aNewFrame|
//
nsresult
NS_NewProgressMeterFrame ( nsIPresShell* aPresShell, nsIFrame** aNewFrame )
{
  NS_PRECONDITION(aNewFrame, "null OUT ptr");
  if (nsnull == aNewFrame) {
    return NS_ERROR_NULL_POINTER;
  }
  nsProgressMeterFrame* it = new (aPresShell) nsProgressMeterFrame(aPresShell);
  if (nsnull == it)
    return NS_ERROR_OUT_OF_MEMORY;

  *aNewFrame = it;
  return NS_OK;
  
} // NS_NewProgressMeterFrame

//
// nsProgressMeterFrame cntr
//
// Init, if necessary
//
nsProgressMeterFrame :: nsProgressMeterFrame (nsIPresShell* aPresShell)
:nsBoxFrame(aPresShell)
{
}

//
// nsProgressMeterFrame dstr
//
// Cleanup, if necessary
//
nsProgressMeterFrame :: ~nsProgressMeterFrame ( )
{
}

NS_IMETHODIMP
nsProgressMeterFrame::SetInitialChildList(nsPresContext* aPresContext,
                                     nsIAtom*        aListName,
                                     nsIFrame*       aChildList)
{ 
  // Set up our initial flexes.
  nsresult rv = nsBoxFrame::SetInitialChildList(aPresContext, aListName, aChildList);
  AttributeChanged(mContent, kNameSpaceID_None, nsHTMLAtoms::value, 0);
  return rv;
}

NS_IMETHODIMP
nsProgressMeterFrame::AttributeChanged(nsIContent* aChild,
                                       PRInt32 aNameSpaceID,
                                       nsIAtom* aAttribute,
                                       PRInt32 aModType)
{
  nsresult rv = nsBoxFrame::AttributeChanged(aChild, aNameSpaceID,
                                             aAttribute, aModType);
  if (NS_OK != rv) {
    return rv;
  }

  // did the progress change?
  if (nsHTMLAtoms::value == aAttribute) {
    nsIFrame* barChild = GetFirstChild(nsnull);
    if (!barChild) return NS_OK;
    nsIFrame* remainderChild = barChild->GetNextSibling();
    if (!remainderChild) return NS_OK;
    nsCOMPtr<nsIContent> remainderContent = remainderChild->GetContent();
    if (!remainderContent) return NS_OK;

    nsAutoString value;
    mContent->GetAttr(kNameSpaceID_None, nsHTMLAtoms::value, value);

    PRInt32 error;
    PRInt32 flex = value.ToInteger(&error);
    if (flex < 0) flex = 0;
    if (flex > 100) flex = 100;

    PRInt32 remainder = 100 - flex;

    nsAutoString leftFlex, rightFlex;
    leftFlex.AppendInt(flex);
    rightFlex.AppendInt(remainder);
    nsWeakFrame weakFrame(this);
    barChild->GetContent()->SetAttr(kNameSpaceID_None, nsXULAtoms::flex, leftFlex, PR_TRUE);
    remainderContent->SetAttr(kNameSpaceID_None, nsXULAtoms::flex, rightFlex, PR_TRUE);

    if (weakFrame.IsAlive()) {
      nsBoxLayoutState state(GetPresContext());
      MarkDirty(state);
    }
  }
  return NS_OK;
}

#ifdef NS_DEBUG
NS_IMETHODIMP
nsProgressMeterFrame::GetFrameName(nsAString& aResult) const
{
  return MakeFrameName(NS_LITERAL_STRING("ProgressMeter"), aResult);
}
#endif
