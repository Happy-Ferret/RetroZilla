/* -*- Mode: Java; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * The contents of this file are subject to the Netscape Public
 * License Version 1.1 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy of
 * the License at http://www.mozilla.org/NPL/
 *
 * Software distributed under the License is distributed on an "AS
 * IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * rights and limitations under the License.
 *
 * The Original Code is Mozilla Communicator client code, released
 * March 31, 1998.
 *
 * The Initial Developer of the Original Code is Netscape
 * Communications Corporation.  Portions created by Netscape are
 * Copyright (C) 1998 Netscape Communications Corporation. All
 * Rights Reserved.
 *
 * Contributor(s):
 *
 * Patrick C. Beard <beard@netscape.com>
 *
 * Alternatively, the contents of this file may be used under the
 * terms of the GNU Public License (the "GPL"), in which case the
 * provisions of the GPL are applicable instead of those above.
 * If you wish to allow use of your version of this file only
 * under the terms of the GPL and not to allow others to use your
 * version of this file under the NPL, indicate your decision by
 * deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL.  If you do not delete
 * the provisions above, a recipient may use your version of this
 * file under either the NPL or the GPL.
 */

import java.util.*;
import java.io.*;

public class Reference {
    int mAddress;
    Type mType;
    Object[] mReferences;

    public Reference(String addr, Type type, Object[] refs) throws NumberFormatException {
        mAddress = Integer.parseInt(addr.substring(2), 16);
        mType = type;
        mReferences = refs;
    }
    
    /**
     * Tests whether ref is an interior pointer into this reference.
     */
    public boolean contains(long address) {
        return (mAddress <= address && address < (mAddress + mType.mSize));
    }
    
	/**
	 * Sorts in order of increasing addresses.
	 */
	public static class ByAddress extends QuickSort.Comparator {
		public int compare(Object obj1, Object obj2) {
			Reference r1 = (Reference) obj1, r2 = (Reference) obj2;
			return (r1.mAddress - r2.mAddress);
		}
	}
	
    public static Reference findNearest(Reference[] sortedRefs, int address) {
		int length = sortedRefs.length;
		int minIndex = 0, maxIndex = length - 1;
		int index = maxIndex / 2;
		while (minIndex <= maxIndex) {
			Reference ref = sortedRefs[index];
			if (address < ref.mAddress) {
				maxIndex = (index - 1);
				index = (minIndex + maxIndex) / 2;
			} else {
				if (address < (ref.mAddress + ref.mType.mSize)) {
					return ref;
				}
				minIndex = (index + 1);
				index = (minIndex + maxIndex) / 2;
			}
		}
		return null;
    }
}
