syntax = "proto2";

package ControlData;

message Attrib
{
	required bytes		name = 1;
	required bytes		value = 2;
}

message Control
{
	required bytes		class = 1;
	repeated Attrib		attribs = 2;
	repeated Control	childs = 3;
}