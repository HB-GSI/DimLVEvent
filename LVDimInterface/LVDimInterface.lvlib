<?xml version='1.0' encoding='UTF-8'?>
<Library LVVersion="14008000">
	<Property Name="Alarm Database Computer" Type="Str">localhost</Property>
	<Property Name="Alarm Database Name" Type="Str">E__Program_Files_National_Instruments_LabVIEW_8_2_data</Property>
	<Property Name="Alarm Database Path" Type="Str">E:\Program Files\National Instruments\LabVIEW 8.2\data</Property>
	<Property Name="Data Lifespan" Type="UInt">3650</Property>
	<Property Name="Database Computer" Type="Str">localhost</Property>
	<Property Name="Database Name" Type="Str">E__Program_Files_National_Instruments_LabVIEW_8_2_data</Property>
	<Property Name="Database Path" Type="Str">E:\Program Files\National Instruments\LabVIEW 8.2\data</Property>
	<Property Name="Enable Alarms Logging" Type="Bool">false</Property>
	<Property Name="Enable Data Logging" Type="Bool">false</Property>
	<Property Name="NI.Lib.Description" Type="Str">DIM has been developed and is copyrighted by CERN under the terms of GPL. DIM is a communication system for distributed / mixed environments, it provides a network transparent inter-process communication layer. For an overview of DIM's functionality and guidance on its usage, please have a look at the DIM web pages www.cern.ch/dim.

This drivers provides an event driven interface to the DIM "C"-interface.

Important:
1) Prior to using this driver, you should download DIM from the DIM webpage, www.cern.ch/dim. Don't forget to start the DIM Name Server and define the environment variable "DIM_DNS_NODE" (see documentation of dim)

2) It may be a good idea to have a look at the documentation of DIM provided at the DIM web pages.

3) The size of (command) services is in prinicple unlimited. However, if you have a DIM client that is subscribing to a DIM server in the same(!) application/task, then the size of services to be transferred between the two is limited to 256kByte. Please note, that transferring data via DIM within the same application is an undocumented feature of DIM and no standard. (Comment: However, this is useful in LabVIEW but use it with care...).

4) One must not use more than one DIM server in the same LabVIEW execution system. As an example, you should not run the application examples of this interface together with the CS framework.

5) For proper operation, the LabVIEW application must start the VI LVDimInterface.dim_operate.vi. This may take a short time time for starting, so allow a delay of about 1000ms (1 second) before subscribing to services (client) or publishing a command service (server). 

6) Windows only: It may be of advantage to define an environment variable DIMWRAPPER_LOGPATH and set it to the name of a folder (!) where the logfile "dimWrapper.log" will be written.

Next to the VIs of this VI tree, a few dlls are required which can be found in ROOTPATH\C\GPL\Projects\DimLVEvent\bin.
- myDimStd.dll (Linux: libmydim.so, PharLap: myDimSPL.dll)
- libDimWrapper.dll (Linux: libDimWrapper.so, PharLap: libDimWrapperSPL.dll)
- msvcr70(d).dll (not required for Linux)

author: Dietrich Beck, GSI
maintainer: Dietrich Beck, GSI

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
For all questions and ideas contact D. Beck.
Last update: 28-Jan-2008

INFO2SF</Property>
	<Property Name="NI.Lib.FriendGUID" Type="Str">0255c025-c2a7-4700-a839-9a16902e87cd</Property>
	<Property Name="NI.Lib.HelpPath" Type="Str"></Property>
	<Property Name="NI.Lib.Icon" Type="Bin">&amp;!#!!!!!!!)!"1!&amp;!!!-!%!!!@````]!!!!"!!%!!!*X!!!*Q(C=\:9R&lt;BMR%%7`!R&gt;RK2M%+H+!4/6_6&lt;D0FCE^6V#\J3\A9A!BH1*%:6L6-7"ALK!L[!L+7SZ8E:V!3A!&amp;="(3M[4_E$.P33ZBK:3XUL8WZYJ/&amp;PS-["`\M&gt;89^J/0`!&gt;.9\P8LX\^P@`1?Z&amp;`6-`Q``&gt;@UL^]XH`O8`\7P[25ZV*$&gt;XC-`K.[X/C&amp;8S@^*?B2^W@]C\\`K`/@`\S\MY0_=:'O6'_,R!*TT):P/N%40&gt;%40&gt;%40&gt;!$0&gt;!$0&gt;!$X&gt;%&gt;X&gt;%&gt;X&gt;%&gt;X&gt;!.X&gt;!.X&gt;"NP#P1B3ZU7&lt;F$EO2*IC2I%C!:$)K32]+4]#1]#1]`F@!E0!F0QJ0Q-%1*4]+4]#1]#1`4F0!E0!F0QJ0Q%+K1:+HI]#1]B&amp;@!%`!%0!&amp;0Q%.+"4Q"1*!M#"Q%!5/"-_A%0!&amp;0Q%.8!5`!%`!%0!%0&lt;A5]!5`!%`!%0%QJKR+&amp;JK`I]""'$I`$Y`!Y0!Y0I?8Q/$Q/D]0D]*"/$I`$YU!Y#:XA)-C:Z!RQ@DA]$A_.("[(R_&amp;R?"Q?8'7(P+R-4^.8&gt;(A-(I0(Y$&amp;Y$"Z#S/!R?!Q?A]@A);Q-(I0(Y$&amp;Y$"Z3S?!R?!Q?!]2)3HI:Q9S*RC"$-(DY+[@&amp;SCZ&amp;)&lt;&amp;3[[=Z(&amp;46![B[M&amp;10D/J"5.VAV9V4X2$6B6:&gt;1.7&amp;58VBV2&gt;2"62.L"J1&gt;;"W0,@9"FND#WS/N6C$4&lt;&amp;*0`8#!X?\H&lt;&lt;&lt;L4;&lt;D&gt;&lt;LN2;,B?&lt;TO&gt;KW6&gt;-UGE[HGEQGB^PK(85I6_/^V.&amp;@X=]_L&gt;LP4[NW6OT&lt;`?SOV``=OJPOOLPZ`0\R^O(DV\O("PPQZ@&lt;5H/&amp;??AVXI^\5`ZI/;`1$T).I/!!!!!!</Property>
	<Property Name="NI.Lib.SourceVersion" Type="Int">335577088</Property>
	<Property Name="NI.Lib.Version" Type="Str">1.17.0.0</Property>
	<Property Name="NI.SortType" Type="Int">3</Property>
	<Property Name="SaveStatePeriod" Type="UInt">0</Property>
	<Property Name="Serialized ACL" Type="Bin">&amp;!#!!!!!!!)!"1!&amp;!!!A1%!!!@````]!!".V&lt;H.J:WZF:#"C?82F)'&amp;S=G&amp;Z!!%!!1!!!!A)!!!!#!!!!!!!!!!</Property>
	<Property Name="Use Data Logging Database" Type="Bool">true</Property>
	<Item Name="Friends List" Type="Friends List">
		<Item Name="CSSystemLib.lvlib" Type="Friended Library" URL="../../../../CSClass/CS_Core/CSSystem/CSSystemLib/CSSystemLib.lvlib"/>
	</Item>
	<Item Name="typedefs" Type="Folder">
		<Property Name="NI.LibItem.Scope" Type="Int">1</Property>
		<Item Name="LVDimInterface.dim service type.ctl" Type="VI" URL="../LVDimInterface.dim service type.ctl"/>
		<Item Name="LVDimInterface.notifier type.ctl" Type="VI" URL="../LVDimInterface.notifier type.ctl"/>
		<Item Name="LVDimInterface.queue type.ctl" Type="VI" URL="../LVDimInterface.queue type.ctl"/>
	</Item>
	<Item Name="public" Type="Folder">
		<Property Name="NI.LibItem.Scope" Type="Int">1</Property>
		<Item Name="client" Type="Folder">
			<Item Name="LVDimInterface.dic_info_service.vi" Type="VI" URL="../LVDimInterface.dic_info_service.vi"/>
			<Item Name="LVDimInterface.dic_info_service_notifier.vi" Type="VI" URL="../LVDimInterface.dic_info_service_notifier.vi"/>
			<Item Name="LVDimInterface.dic_info_service_queue.vi" Type="VI" URL="../LVDimInterface.dic_info_service_queue.vi"/>
			<Item Name="LVDimInterface.dic_release_service.vi" Type="VI" URL="../LVDimInterface.dic_release_service.vi"/>
			<Item Name="LVDimInterface.dic_info_service_once.vi" Type="VI" URL="../LVDimInterface.dic_info_service_once.vi"/>
			<Item Name="LVDimInterface.dic_command_service.vi" Type="VI" URL="../LVDimInterface.dic_command_service.vi"/>
			<Item Name="LVDimInterface.dic_command_service_local.vi" Type="VI" URL="../LVDimInterface.dic_command_service_local.vi"/>
		</Item>
		<Item Name="server" Type="Folder">
			<Item Name="LVDimInterface.dis_add_command.vi" Type="VI" URL="../LVDimInterface.dis_add_command.vi"/>
			<Item Name="LVDimInterface.dis_remove_command.vi" Type="VI" URL="../LVDimInterface.dis_remove_command.vi"/>
			<Item Name="LVDimInterface.dis_remove_service.vi" Type="VI" URL="../LVDimInterface.dis_remove_service.vi"/>
			<Item Name="LVDimInterface.dis_set_quality.vi" Type="VI" URL="../LVDimInterface.dis_set_quality.vi"/>
			<Item Name="LVDimInterface.dis_set_time_stamp.vi" Type="VI" URL="../LVDimInterface.dis_set_time_stamp.vi"/>
			<Item Name="LVDimInterface.dis_start_serving.vi" Type="VI" URL="../LVDimInterface.dis_start_serving.vi"/>
			<Item Name="LVDimInterface.dis_stop_serving.vi" Type="VI" URL="../LVDimInterface.dis_stop_serving.vi"/>
			<Item Name="LVDimInterface.dis_update_service.vi" Type="VI" URL="../LVDimInterface.dis_update_service.vi"/>
			<Item Name="LVDimInterface.dis_add_command_notifier.vi" Type="VI" URL="../LVDimInterface.dis_add_command_notifier.vi"/>
			<Item Name="LVDimInterface.dis_add_command_queue.vi" Type="VI" URL="../LVDimInterface.dis_add_command_queue.vi"/>
			<Item Name="LVDimInterface.dis_add_service.vi" Type="VI" URL="../LVDimInterface.dis_add_service.vi"/>
		</Item>
		<Item Name="common" Type="Folder">
			<Item Name="LVDimInterface.dim_disable_security.vi" Type="VI" URL="../LVDimInterface.dim_disable_security.vi"/>
			<Item Name="LVDimInterface.dim_enable_security.vi" Type="VI" URL="../LVDimInterface.dim_enable_security.vi"/>
			<Item Name="LVDimInterface.dim_get_info.vi" Type="VI" URL="../LVDimInterface.dim_get_info.vi"/>
			<Item Name="LVDimInterface.dim_get_service_info.vi" Type="VI" URL="../LVDimInterface.dim_get_service_info.vi"/>
			<Item Name="LVDimInterface.dim_operate.vi" Type="VI" URL="../LVDimInterface.dim_operate.vi"/>
			<Item Name="LVDimInterface.dim_unfold_data.vi" Type="VI" URL="../LVDimInterface.dim_unfold_data.vi"/>
			<Item Name="LVDimInterface.dim_unfold_data_info.vi" Type="VI" URL="../LVDimInterface.dim_unfold_data_info.vi"/>
			<Item Name="LVDimInterface.error_message.vi" Type="VI" URL="../LVDimInterface.error_message.vi"/>
			<Item Name="LVDimInterface.get library version.vi" Type="VI" URL="../LVDimInterface.get library version.vi"/>
		</Item>
		<Item Name="examples" Type="Folder">
			<Item Name="LVDimInterface.application example client.vi" Type="VI" URL="../LVDimInterface.application example client.vi"/>
			<Item Name="LVDimInterface.application example serv buffer.vi" Type="VI" URL="../LVDimInterface.application example serv buffer.vi"/>
			<Item Name="LVDimInterface.application example serv command.vi" Type="VI" URL="../LVDimInterface.application example serv command.vi"/>
			<Item Name="LVDimInterface.application example serv command_double_array.vi" Type="VI" URL="../LVDimInterface.application example serv command_double_array.vi"/>
			<Item Name="LVDimInterface.application example serv command_integer_double.vi" Type="VI" URL="../LVDimInterface.application example serv command_integer_double.vi"/>
		</Item>
		<Item Name="TestTools" Type="Folder">
			<Item Name="LVDimInterface.test ping pong client.vi" Type="VI" URL="../TestTools/LVDimInterface.test ping pong client.vi"/>
			<Item Name="LVDimInterface.test ping pong server.vi" Type="VI" URL="../TestTools/LVDimInterface.test ping pong server.vi"/>
			<Item Name="LVDimInterface.test simple client.vi" Type="VI" URL="../TestTools/LVDimInterface.test simple client.vi"/>
			<Item Name="LVDimInterface.test simple server.vi" Type="VI" URL="../TestTools/LVDimInterface.test simple server.vi"/>
		</Item>
	</Item>
	<Item Name="private" Type="Folder">
		<Property Name="NI.LibItem.Scope" Type="Int">2</Property>
		<Item Name="LVDimInterface.dim_dispatcher.vi" Type="VI" URL="../LVDimInterface.dim_dispatcher.vi"/>
		<Item Name="LVDimInterface.dim_fire_occurrence.vi" Type="VI" URL="../LVDimInterface.dim_fire_occurrence.vi"/>
		<Item Name="LVDimInterface.dim_get_occurrence_ref.vi" Type="VI" URL="../LVDimInterface.dim_get_occurrence_ref.vi"/>
		<Item Name="LVDimInterface.dim_set_occurrence_ref.vi" Type="VI" URL="../LVDimInterface.dim_set_occurrence_ref.vi"/>
		<Item Name="LVDimInterface.dim_semaphore.vi" Type="VI" URL="../LVDimInterface.dim_semaphore.vi"/>
		<Item Name="LVDimInterface.dim_set_DNS_node.vi" Type="VI" URL="../LVDimInterface.dim_set_DNS_node.vi"/>
	</Item>
	<Item Name="forFriendsOnly" Type="Folder">
		<Property Name="NI.LibItem.Scope" Type="Int">4</Property>
		<Item Name="LVDimInterface.release.vi" Type="VI" URL="../LVDimInterface.release.vi"/>
		<Item Name="LVDimInterface.acquire.vi" Type="VI" URL="../LVDimInterface.acquire.vi"/>
	</Item>
	<Item Name="supportFiles" Type="Folder">
		<Property Name="NI.LibItem.Scope" Type="Int">1</Property>
		<Item Name="performance_DIM_LV2009.xls" Type="Document" URL="../../performance_DIM_LV2009.xls"/>
		<Item Name="libDimWrapper.dll" Type="Document" URL="../../bin/libDimWrapper.dll"/>
		<Item Name="libDimWrapperSPL.dll" Type="Document" URL="../../bin/libDimWrapperSPL.dll"/>
		<Item Name="libDimWrapper.so" Type="Document" URL="../../bin/libDimWrapper.so"/>
		<Item Name="msvcp100.dll" Type="Document" URL="../../bin/msvcp100.dll"/>
		<Item Name="msvcr100.dll" Type="Document" URL="../../bin/msvcr100.dll"/>
		<Item Name="myDimStd.dll" Type="Document" URL="../../bin/myDimStd.dll"/>
		<Item Name="myDimSPL.dll" Type="Document" URL="../../bin/myDimSPL.dll"/>
	</Item>
	<Item Name="LVDimInterface.VI-Tree.vi" Type="VI" URL="../LVDimInterface.VI-Tree.vi"/>
</Library>
