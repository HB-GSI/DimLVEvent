<?xml version='1.0' encoding='UTF-8'?>
<Library LVVersion="15008000">
	<Property Name="Alarm Database Computer" Type="Str">localhost</Property>
	<Property Name="Alarm Database Name" Type="Str">E__Program_Files_National_Instruments_LabVIEW_8_2_data</Property>
	<Property Name="Alarm Database Path" Type="Str">E:\Program Files\National Instruments\LabVIEW 8.2\data</Property>
	<Property Name="Data Lifespan" Type="UInt">3650</Property>
	<Property Name="Database Computer" Type="Str">localhost</Property>
	<Property Name="Database Name" Type="Str">E__Program_Files_National_Instruments_LabVIEW_8_2_data</Property>
	<Property Name="Database Path" Type="Str">E:\Program Files\National Instruments\LabVIEW 8.2\data</Property>
	<Property Name="Enable Alarms Logging" Type="Bool">false</Property>
	<Property Name="Enable Data Logging" Type="Bool">false</Property>
	<Property Name="NI.Lib.Description" Type="Str">This library can be used to subsribesto DIM services. The main idea is to provide an easy access to DIM as an example for GUIs. The library is not intended for progamming code aimed at high performance. 

The "LVDimInterface.dim_operate" routine of the LVDimInterface library must be started prior to using this DimIndicators provided in this library. Subscribing to a DIM service is done by invoking the "dic_info_service" routine. Releasing a DIM service is done by invoking the "dic_release_service" routine of THIS library. 

Important: You may only use the data types that are available in the "typedefs" folder.

License Agreement for this software:

Copyright (C)
Gesellschaft für Schwerionenforschung, GSI
Planckstr. 1
64291 Darmstadt
Germany

Contact: D.Beck@gsi.de 

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see &lt;http://www.gnu.org/licenses/&gt;.

Gesellschaft für Schwerionenforschung, GSI
Planckstr. 1, 64291 Darmstadt, Germany
For all questions and ideas contact: D.Beck@gsi.de or H.Brand@gsi.de. 
Last update: 28-Jan-2008

INFO2SF
</Property>
	<Property Name="NI.Lib.FriendGUID" Type="Str">660d01fe-f8a6-448b-8b15-649a6c765116</Property>
	<Property Name="NI.Lib.HelpPath" Type="Str"></Property>
	<Property Name="NI.Lib.Icon" Type="Bin">&amp;1#!!!!!!!)!"1!&amp;!!!-!%!!!@````]!!!!"!!%!!!*'!!!*Q(C=\:3^=1*"$%9`?RRYRB%&gt;'&amp;J1!QYIQ4@DS+&amp;;)#5E&gt;;D%"&gt;!#,;A&amp;!BKAB@/\27!T9X/*0?/!812XH\43WR^7;OV?OF-`VBYONM&amp;`C$I'HA&lt;U*`^8^5&gt;`@]L4;F&lt;F-`^:`W&lt;]4`ZDXD0P:`[R_6XWDT6&gt;&lt;&amp;@`V@_X`P(DO2Q`R(`&lt;J*N'CC57G'07:N=H?K)H?K)H?K)(?K!(?K!(?K!\OK-\OK-\OK-&lt;OK%&lt;OK%&lt;OK'XDCZUI1O&gt;65G+*Y73J%G#*"A5*6]*4]+4]#1]P#LB38A3HI1HY3&amp;%#5`#E`!E0!E0QZ4Q*$Q*4]+4]*#KE74L[0!E0+28Q"0Q"$Q"4]"$315]!5"1,%A=*!&amp;$A4.Y#(A#HI#(2Q5]!5`!%`!%0,A6]!1]!5`!%`!QJ+V+.*KBI].$'DE]$I`$Y`!Y0+37Q_0Q/$Q/D].$/4E]$I]$Y22UEI-A:Z!4Y,QY0!Y00X*Y("[(R_&amp;R?(#V(@+W-A0.U.(B-8A-(I0(Y$&amp;Y3#'$R_!R?!Q?AY?U-HA-(I0(Y$&amp;Y+#7$R_!R?!Q1ISDF:31T"BJ"BG$Q]'GHR&gt;IO23/RVOOP?4CIKA/I/FCK![-[#+I.6GW=;E.5#[V;1.8#K#;MGIA+5&amp;69F6!6K$X@7WS$L&lt;%6NM![&lt;)\.M-EQ^*=$^`O^NNON.JO.VOOV6KO6&amp;IO&amp;OK\4@$\8&lt;$&lt;4:$)ZX6;0^%/\/&gt;Z,3Z\@HZ[G&lt;]*?8X:P84=&gt;\,V\XAW_Q9ZRPWG(?_E`X)W[6@`FGG?.0A$4['0.!!!!!!</Property>
	<Property Name="NI.Lib.SourceVersion" Type="Int">352354304</Property>
	<Property Name="NI.Lib.Version" Type="Str">1.17.0.0</Property>
	<Property Name="NI.SortType" Type="Int">3</Property>
	<Property Name="SaveStatePeriod" Type="UInt">0</Property>
	<Property Name="Serialized ACL" Type="Bin">&amp;1#!!!!!!!)!"1!&amp;!!!A1%!!!@````]!!".V&lt;H.J:WZF:#"C?82F)'&amp;S=G&amp;Z!!%!!1!!!!A)!!!!#!!!!!!!!!!</Property>
	<Property Name="Use Data Logging Database" Type="Bool">true</Property>
	<Item Name="Friends List" Type="Friends List">
		<Item Name="LVDimInterface.lvlib" Type="Friended Library" URL="../../LVDimInterface/LVDimInterface.lvlib"/>
	</Item>
	<Item Name="typedefs" Type="Folder">
		<Property Name="NI.LibItem.Scope" Type="Int">1</Property>
		<Item Name="DimIndicators.anyType.ctl" Type="VI" URL="../DimIndicators.anyType.ctl"/>
		<Item Name="DimIndicators.numericArrayType.ctl" Type="VI" URL="../DimIndicators.numericArrayType.ctl"/>
		<Item Name="DimIndicators.numericType.ctl" Type="VI" URL="../DimIndicators.numericType.ctl"/>
	</Item>
	<Item Name="public" Type="Folder">
		<Property Name="NI.LibItem.Scope" Type="Int">1</Property>
		<Item Name="DimIndicators.dic_info_service.vi" Type="VI" URL="../DimIndicators.dic_info_service.vi"/>
		<Item Name="DimIndicators.dic_release_service.vi" Type="VI" URL="../DimIndicators.dic_release_service.vi"/>
		<Item Name="DimIndicators.get library version.vi" Type="VI" URL="../DimIndicators.get library version.vi"/>
		<Item Name="DimIndicators.get_service_info.vi" Type="VI" URL="../DimIndicators.get_service_info.vi"/>
	</Item>
	<Item Name="private" Type="Folder">
		<Property Name="NI.LibItem.Scope" Type="Int">2</Property>
		<Item Name="DimIndicators.actionType.ctl" Type="VI" URL="../DimIndicators.actionType.ctl"/>
		<Item Name="DimIndicators.dataType.ctl" Type="VI" URL="../DimIndicators.dataType.ctl"/>
		<Item Name="DimIndicators.FGV_core.vi" Type="VI" URL="../DimIndicators.FGV_core.vi"/>
	</Item>
	<Item Name="forFriendsOnly" Type="Folder">
		<Property Name="NI.LibItem.Scope" Type="Int">4</Property>
		<Item Name="DimIndicators.dispatcher.vi" Type="VI" URL="../DimIndicators.dispatcher.vi"/>
	</Item>
	<Item Name="examples" Type="Folder">
		<Property Name="NI.LibItem.Scope" Type="Int">1</Property>
		<Item Name="DimIndicators.application example double.vi" Type="VI" URL="../DimIndicators.application example double.vi"/>
		<Item Name="DimIndicators.application example double array.vi" Type="VI" URL="../DimIndicators.application example double array.vi"/>
		<Item Name="DimIndicators.application example any type.vi" Type="VI" URL="../DimIndicators.application example any type.vi"/>
	</Item>
	<Item Name="DimIndicators.VI-Tree.vi" Type="VI" URL="../DimIndicators.VI-Tree.vi"/>
</Library>
