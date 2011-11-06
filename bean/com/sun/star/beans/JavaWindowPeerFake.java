/**************************************************************
 * 
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 * 
 *************************************************************/



package com.sun.star.beans;

import com.sun.star.awt.*;

/** <p>Class to pass the system window handle to the OpenOffice.org toolkit.</p>
 *  @deprecated
 */
/* package */ class JavaWindowPeerFake
	implements XSystemDependentWindowPeer, XWindowPeer
{

	protected long hWindow;
	protected int localSystemType;

	/** Create the faked window peer.
	 * @param _hWindow the system handle to the window.
	 * @param _systemType specifies the system type.
	 */
	public JavaWindowPeerFake(long _hWindow, int _systemType)
	{
		hWindow = _hWindow;
		localSystemType = _systemType;
	}

	/** <p>Implementation of XSystemDependentWindowPeer (that's all we really need)</p>
	 *  This method is called back from the OpenOffice.org toolkit to retrieve the system data.
	 */
    public Object getWindowHandle(/*IN*/byte[] ProcessId, /*IN*/short SystemType) 
		throws com.sun.star.uno.RuntimeException
	{

		if (SystemType == localSystemType) {
			return new Integer((int)hWindow);
		}
		else return null;
	}
	
	/** not really neaded.
	 */
    public XToolkit getToolkit()
		throws com.sun.star.uno.RuntimeException
	{
		return null;
	}

	/** not really neaded.
	 */
    public void setPointer(/*IN*/XPointer Pointer)
		throws com.sun.star.uno.RuntimeException
	{
	}

	/** not really neaded.
	 */
    public void setBackground(/*IN*/int Color)
		throws com.sun.star.uno.RuntimeException
	{
	}

	/** not really neaded.
	 */
    public void invalidate(/*IN*/short Flags)
		throws com.sun.star.uno.RuntimeException
	{
	}

	/** not really neaded.
	 */
    public void invalidateRect(/*IN*/com.sun.star.awt.Rectangle Rect, /*IN*/short Flags)
		throws com.sun.star.uno.RuntimeException
	{
	}

	/** not really neaded.
	 */
    public void dispose()
		throws com.sun.star.uno.RuntimeException
	{
	}

	/** not really neaded.
	 */
    public void addEventListener(/*IN*/com.sun.star.lang.XEventListener xListener)
		throws com.sun.star.uno.RuntimeException
	{
	}

	/** not really neaded.
	 */
    public void removeEventListener(/*IN*/com.sun.star.lang.XEventListener aListener)
		throws com.sun.star.uno.RuntimeException
	{
	}
}

