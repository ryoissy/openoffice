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

package installer;

import java.io.*;
import java.util.*;
import java.util.zip.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

public class ZipData
{
    public ZipData(String file) {
    }
    
    public boolean extractEntry(String entry, String destination,
        JLabel statusLabel) {

        OutputStream out = null;
        InputStream in = null;

        System.out.println("Copying: " + entry);
        System.out.println("To: " + destination);

	if (statusLabel != null) {
		statusLabel.setText("Copying " + entry);
	}

        String entryName;
        if (entry.lastIndexOf("/") != -1) {
            entryName = entry.substring(entry.lastIndexOf("/") + 1);
        }
        else {
            entryName = entry;
        }

        String destName;
        if (destination.lastIndexOf(File.separator) != -1) {
            destName = destination.substring(destination.lastIndexOf(File.separator) + 1);
        }
        else {
            destName = destination;
        }

        if (!destName.equals(entryName))
            destination = destination.concat(entryName);
	    
        System.out.println("Unzipping " + entry + " to " + destination);

	try {
            out = new FileOutputStream(destination);
        }
        catch (IOException ioe) {
            System.err.println("Error opening " + destination +
                ": " + ioe.getMessage());

            if (statusLabel != null)
                statusLabel.setText("Error opening" + destination +
                    "see SFramework.log for more information");

            return false;
        }

        if (entry.startsWith("/") == false)
            entry = "/" + entry;

        in = this.getClass().getResourceAsStream(entry);
        if (in == null) {
            System.err.println("File " + entry + " not found in jar file");

            if (statusLabel != null)
                statusLabel.setText("Failed extracting " + entry +
                    "see SFramework.log for more information");

            return false;
        }

        try {
            byte[] bytes = new byte[1024];
            int len;

            while ((len = in.read(bytes)) != -1)
                out.write(bytes, 0, len);
        }
        catch (IOException ioe) {
            System.err.println("Error writing " + destination + ": " +
                ioe.getMessage());

            if (statusLabel != null)
                statusLabel.setText("Failed writing " + destination +
                    "see SFramework.log for more information");
            return false;
        }
        finally {
            try {
                in.close();
                out.close();
            }
            catch (IOException ioe) {
            }
        }
        return true;
    }
}
