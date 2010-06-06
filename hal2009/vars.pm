package vars;

use 5.006;

our $VERSION = '2.01';


sub import {
    my $callpack = caller;
    my ($pack, @imports) = @_;
    my ($sym, $ch);
    foreach (@imports) {
        if (($ch, $sym) = /^([\$\@\%\*\&])(.+)/) {
	    if ($sym =~ /\W/) {
		# time for a more-detailed check-up
		if ($sym =~ /^\w+[[{].*[]}]$/) {
		    warn("Can't declare individual elements of hash or array");
		} elsif (warnings::enabled() and length($sym) == 1 and $sym !~ tr/a-zA-Z//) {
		    warn("No need to declare built-in vars");
		} elsif  (($^H &= strict::bits('vars'))) {
		    warn("'$_' is not a valid variable name under strict vars");
		}
	    }
	    $sym = "${callpack}::$sym" unless $sym =~ /::/;
	    *$sym =
		(  $ch eq "\$" ? \$$sym
		 : $ch eq "\@" ? \@$sym
		 : $ch eq "\%" ? \%$sym
		 : $ch eq "\*" ? \*$sym
		 : $ch eq "\&" ? \&$sym 
		 : do {
		     warn("'$_' is not a valid variable name");
		 });
	} else {
	    warn("'$_' is not a valid variable name");
	}
    }
};

1;
__END__

