//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//
simple Txc3
{
	parameters:
		@display("i=block/routing"); // add a default icon
    gates:
        input in;
        output out;
}

// two instances tic and toc of Txc2 connected both ways
// Tic and Toc pass message to each other
network Tictoc3
{
    @display("bgb=239,232");
    submodules:
        tic: Txc3 {
        	parameters:
        		@display("i=,cyan");
            	@display("p=39,185");
        }
        toc: Txc3 {
        	parameters:
        		@display("i=,gold");
            	@display("p=183,42");
        }
    connections:
        tic.out --> {  delay = 100ms; } --> toc.in;
        tic.in <-- {  delay = 100ms; } <-- toc.out;
}
