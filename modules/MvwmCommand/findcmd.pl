#!/usr/bin/perl
# Find fvwm commands from functions.c struct functions
# Written by Toshi Isogai

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

sub getcmd {
  $/ = "\n\n";
  while(<STDIN>) {
    #find "func_t func_config[] =" in various spacing
    if (s/func_t.*\[\]\s*=(\s|\n)*\{// ) {
      return listcmd();
    }
  }
  print stderr "Can't find func_t\n";
  exit 1;
}

sub listcmd {
  my($cmd,@cmd);

  while (/CMD_ENTRY/) {
    $old = $_;
    s/.*CMD_ENTRY[^,]*,\s*CMD_([^,]+).*/\1/s;
    s/\s*$//;
    if (!/^\+\s*$/) {
      push @cmd, $_;
    }
    $_ = $old;
    s/(.*)CMD_ENTRY.*/\1/s;
  }
  @cmd;
}

sub create_pm {
  my( @cmd, $i, @ln );
  my( $fvwmdir ) = $ARGV[0];
  my( $pm ) = "MvwmCommand.pm";

  @cmd = getcmd();

  if ($ARGV[1] eq "-sh") {
    print "# MvwmCommand.sh\n";
    print "# Collection of fvwm builtin commands for MvwmCommand\n";
    print "#\n";
    print "alias MvwmCommand=\'$fvwmdir/MvwmCommand\'\n";

    for( $i=0; $i<=$#cmd; $i++) {
      print  "$cmd[$i] () {\n";
      print  "\tMvwmCommand \"$cmd[$i] \$\*\"\n";
      print  "}\n";
    }

    print "AM () { \n";
    print "	MvwmCommand \"+ \$\*\"\n";
    print "}\n";
  }
  else {
    print  "# $pm\n";
    print  "# Collection of fvwm builtin commands for MvwmCommand\n";
    print  "package MvwmCommand;\nuse Exporter;\n";
    print  "\@ISA=qw(Exporter);\n\@EXPORT=qw(";
    for( $i=0; $i<=$#cmd; $i++) {
      if( $i % 5 == 0 ) {
	print  "\n  $cmd[$i]";
      }else{
	print  " $cmd[$i]";
      }
    }
    print  "\n);\n";
    print  "\nsub MvwmCommand { system \"$fvwmdir/MvwmCommand '\@_'\"}\n\n";
    foreach $i (@cmd) {
      print  "sub $i { MvwmCommand \"$i \@_ \" }\n";
    }
    print  "sub AM { MvwmCommand \"+ \@_ \" }\n";
    print  "1;\n";
  }
}

create_pm();
