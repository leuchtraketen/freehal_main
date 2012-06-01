while (<STDIN>) {

s/[<]\[/[/igm;
s/\][>]/]/igm;
s/\$0/\\\\1/igm;
s/\$1/\\\\2/igm;
s/\$2/\\\\3/igm;
s/do regex with var input: \/(.*?)\/ -> "(.*?)" :global:i/regex_ireplace(str, "$1", "$2");/igm;
s/do regex with var input: \/(.*?)\/ -> "(.*?)" :global/regex_replace(str, "$1", "$2");/igm;
s/done/}/igm;
s/ do$/) {/igm;
s/if /if (/igm;
s/var (.*?) match.?.? \/(.*?)\//regex_find($1, "$2")/igm;
s/var (.*?) match.?.? "(.*?)"/algo::contains($1, "$2")/igm;
s/set var (.*?) to /$1 = /igm;
s/set new var (.*?) to /string $1 = /igm;
s/for( each)? /foreach (/igm;

print;

}
