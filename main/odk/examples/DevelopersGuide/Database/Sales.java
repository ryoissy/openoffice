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



import java.io.*;

//	import com.sun.star.comp.helper.RegistryServiceFactory;
//	import com.sun.star.comp.servicemanager.ServiceManager;
//	import com.sun.star.lang.XMultiServiceFactory;
//	import com.sun.star.lang.XServiceInfo;
import com.sun.star.lang.XComponent;
//	import com.sun.star.bridge.XUnoUrlResolver;
import com.sun.star.uno.*;
import com.sun.star.util.Date;
import com.sun.star.beans.XPropertySet;
import com.sun.star.container.XNameAccess;
import com.sun.star.sdbc.*;
    
public class Sales
{
	private XConnection con;

	public Sales(XConnection connection )
	{
		con = connection;
	}
	// create the table sales.
	public void createSalesTable() throws com.sun.star.uno.Exception
	{
		String createTableSales = "CREATE TABLE SALES " +
				"(SALENR INTEGER NOT NULL, " +
				" COS_NR INTEGER, " +
				" SNR INTEGER, " +
				" NAME VARCHAR(50)," +
				" SALEDATE DATE," +
				" PRICE FLOAT(10), " +
				" PRIMARY KEY(SALENR)" +
				" )";
		XStatement stmt = con.createStatement();
		stmt.executeUpdate( createTableSales );
	}

	// drop the table sales.
	public void dropSalesTable() throws com.sun.star.uno.Exception
	{
		String createTableSalesman = "DROP TABLE SALES ";
		XStatement stmt = con.createStatement();
		stmt.executeUpdate( createTableSalesman );
	}

	// insert data into the table sales.
	public void insertDataIntoSales() throws com.sun.star.uno.Exception
	{
		XStatement stmt = con.createStatement();
		stmt.executeUpdate("INSERT INTO SALES " + 
				"VALUES (1, '100', '1','Linux','2001-02-12',15)");
		stmt.executeUpdate("INSERT INTO SALES " +
				"VALUES (2, '101', '2','Beef','2001-10-18',15.78)");
		stmt.executeUpdate("INSERT INTO SALES " +
				"VALUES (3, '104', '4','orange juice','2001-08-09',1.5)");
	}

	// update the table sales with a prepared statement.
	public void updateSales() throws com.sun.star.uno.Exception
	{
		XStatement stmt = con.createStatement();
		String updateString =	"UPDATE SALES " +
								"SET PRICE = 30 " + 
								"WHERE SALENR = 1";
		stmt.executeUpdate(updateString);
	}

	// retrieve the data of the table sales.
	public void retrieveSalesData() throws com.sun.star.uno.Exception
	{
		XStatement stmt = con.createStatement();
		String query =	"SELECT NAME, PRICE FROM SALES " +
						"WHERE SALENR = 1";
		XResultSet rs = stmt.executeQuery(query);
		XRow      row = (XRow)UnoRuntime.queryInterface(XRow.class, rs);
		while (rs.next()) {
				String s = row.getString(1);
				float  n = row.getFloat(2);
				System.out.println("The current price for " + s + " is: $" + n + ".");
		}
	}

	// create a scrollable resultset.
	public void retrieveSalesData2() throws com.sun.star.uno.Exception
	{
		// example for a programmatic way to do updates. This doesn't work with adabas.
		XStatement stmt = con.createStatement();
		XPropertySet xProp = (XPropertySet)UnoRuntime.queryInterface(XPropertySet.class,stmt);

		xProp.setPropertyValue("ResultSetType", new java.lang.Integer(ResultSetType.SCROLL_INSENSITIVE));
		xProp.setPropertyValue("ResultSetConcurrency", new java.lang.Integer(ResultSetConcurrency.UPDATABLE));

		XResultSet srs = stmt.executeQuery("SELECT NAME, PRICE FROM SALES");
		XRow       row = (XRow)UnoRuntime.queryInterface(XRow.class,srs);

		srs.afterLast();
		while (srs.previous()) {
			String name = row.getString(1);
			float price = row.getFloat(2);
			System.out.println(name + "     " + price);
		}

		srs.last();
		XRowUpdate updateRow = (XRowUpdate)UnoRuntime.queryInterface(XRowUpdate.class,srs);
		updateRow.updateFloat(2, (float)0.69);

		XResultSetUpdate updateRs = ( XResultSetUpdate )UnoRuntime.queryInterface(
            XResultSetUpdate.class,srs);
		updateRs.updateRow(); // this call updates the data in DBMS

		srs.last();
		updateRow.updateFloat(2, (float)0.99);
		updateRs.cancelRowUpdates();
		updateRow.updateFloat(2, (float)0.79);
		updateRs.updateRow();
	}

	// inserts a row programmatically.
	public void insertRow() throws com.sun.star.uno.Exception
	{
		// example for a programmatic way to do updates. This doesn't work with adabas.
		XStatement stmt = con.createStatement();
//		stmt.executeUpdate("INSERT INTO SALES " +
//                   "VALUES (4, 102, 5, 'FTOP Darjeeling tea', '2002-01-02',150)");
//
//		stmt = con.createStatement();
		XPropertySet xProp = (XPropertySet)UnoRuntime.queryInterface(XPropertySet.class,stmt);
		xProp.setPropertyValue("ResultSetType", new java.lang.Integer(ResultSetType.SCROLL_INSENSITIVE));
		xProp.setPropertyValue("ResultSetConcurrency", new java.lang.Integer(ResultSetConcurrency.UPDATABLE));
		XResultSet rs = stmt.executeQuery("SELECT * FROM SALES");
		XRow       row = (XRow)UnoRuntime.queryInterface(XRow.class,rs);

		// insert a new row
		XRowUpdate updateRow = (XRowUpdate)UnoRuntime.queryInterface(XRowUpdate.class,rs);
		XResultSetUpdate updateRs = ( XResultSetUpdate )UnoRuntime.	queryInterface(XResultSetUpdate.class,rs);
		updateRs.moveToInsertRow();
		updateRow.updateInt(1, 4);
		updateRow.updateInt(2, 102);
		updateRow.updateInt(3, 5);
		updateRow.updateString(4, "FTOP Darjeeling tea");
		updateRow.updateDate(5, new Date((short)1,(short)2,(short)2002));
		updateRow.updateFloat(6, 150);
		updateRs.insertRow();
	}

	// deletes a row programmatically.
	public void deleteRow() throws com.sun.star.uno.Exception
	{
		// example for a programmatic way to do updates. This doesn't work with adabas.
		XStatement stmt = con.createStatement();
		XPropertySet xProp = (XPropertySet)UnoRuntime.queryInterface(XPropertySet.class,stmt);
		xProp.setPropertyValue("ResultSetType", new java.lang.Integer(ResultSetType.SCROLL_INSENSITIVE));
		xProp.setPropertyValue("ResultSetConcurrency", new java.lang.Integer(ResultSetConcurrency.UPDATABLE));
		XResultSet rs = stmt.executeQuery("SELECT * FROM SALES");
		XRow       row = (XRow)UnoRuntime.queryInterface(XRow.class,rs);

		XResultSetUpdate updateRs = ( XResultSetUpdate )UnoRuntime.	queryInterface(XResultSetUpdate.class,rs);
		// move to the inserted row
		rs.absolute(4);
		updateRs.deleteRow();
	}

	// refresh a row
	public void refreshRow() throws com.sun.star.uno.Exception
	{
		// example for a programmatic way to do updates. This doesn't work with adabas.
		// first we need the 4 row
		insertRow();

		XStatement stmt = con.createStatement();
		XPropertySet xProp = (XPropertySet)UnoRuntime.queryInterface(XPropertySet.class,stmt);
		xProp.setPropertyValue("ResultSetType", new java.lang.Integer(ResultSetType.SCROLL_INSENSITIVE));
		xProp.setPropertyValue("ResultSetConcurrency", new java.lang.Integer(ResultSetConcurrency.READ_ONLY));
		XResultSet rs = stmt.executeQuery("SELECT NAME, PRICE FROM SALES");
		XRow       row = (XRow)UnoRuntime.queryInterface(XRow.class, rs);
		rs.absolute(4);
		float price1 = row.getFloat(2);

		// modifiy the 4 row
		XRowUpdate updateRow = (XRowUpdate)UnoRuntime.queryInterface(XRowUpdate.class,rs);
		XResultSetUpdate updateRs = ( XResultSetUpdate )UnoRuntime.	queryInterface(XResultSetUpdate.class,rs);
		updateRow.updateFloat(2, 150);
		updateRs.updateRow();
		// repositioning
		rs.absolute(4);
		rs.refreshRow();
		float price2 = row.getFloat(2);
		if (price2 != price1) {
			System.out.println("Prices are different.");
		}
		else
			System.out.println("Prices are equal.");
		deleteRow();
	}

	// displays the column names
	public void displayColumnNames() throws com.sun.star.uno.Exception
	{
		XStatement stmt = con.createStatement();
		XPropertySet xProp = (XPropertySet)UnoRuntime.queryInterface(XPropertySet.class,stmt);
		xProp.setPropertyValue("ResultSetType", new java.lang.Integer(ResultSetType.SCROLL_INSENSITIVE));
		xProp.setPropertyValue("ResultSetConcurrency", new java.lang.Integer(ResultSetConcurrency.READ_ONLY));
		XResultSet rs = stmt.executeQuery("SELECT NAME, PRICE FROM SALES");
		XResultSetMetaDataSupplier xRsMetaSup = (XResultSetMetaDataSupplier)
            UnoRuntime.queryInterface(XResultSetMetaDataSupplier.class,rs);
		XResultSetMetaData xRsMetaData =  xRsMetaSup.getMetaData();
		int nColumnCount =  xRsMetaData.getColumnCount();
		for(int i=1 ; i <= nColumnCount ; ++i)
		{
		   System.out.println("Name: " + xRsMetaData.getColumnName(i) + " Type: " +
                              xRsMetaData.getColumnType(i));
		}
	}
}

