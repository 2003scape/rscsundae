#!/bin/sh

if curl -V 2>&1 >/dev/null; then
	USE_FETCH=curl
elif wget -V 2>&1 >/dev/null; then
	USE_FETCH=wget
elif [ "$(uname -s)" = "FreeBSD" ]; then
	# don't see a way to test fetch
	USE_FETCH=fetch
elif ftp -? 2>&1 >/dev/null; then
	USE_FETCH=ftp
else
	printf "Please install curl and make sure it's in your PATH.\n" >/dev/stderr
	exit 1
fi

if echo "a" | sha256sum 2>/dev/null >/dev/null; then
	CKSUM="sha256sum -b"
elif echo "a" | gsha256sum 2>/dev/null >/dev/null; then
	CKSUM="gsha256sum -b"
elif echo "a" | sha256 -q 2>/dev/null >/dev/null; then
	CKSUM="sha256 -q"
elif echo "a" | cksum -a sha256 -q 2>/dev/null; then
	CKSUM="cksum -a sha256 -q"
else
	printf "Please install a sha256sum utility and make sure " >/dev/stderr
	printf "it's in your PATH.\n" >/dev/stderr
	exit 1
fi

get_url()
{
	if [ ${USE_FETCH} = "curl" ]; then
		curl -OJL "$1"
	elif [ ${USE_FETCH} = "wget" ]; then
		wget "$1"
	elif [ ${USE_FETCH} = "fetch" ]; then
		fetch "$1"
	elif [ ${USE_FETCH} = "ftp" ]; then
		ftp "$1"
	fi
}

if ! [ -e config46.jag ]; then
	printf "fetching config46.jag\n"
	get_url https://web.archive.org/web/20011212112437/http://66.28.11.53:80/client/config46.jag
fi
printf "verifying config46.jag\n"
hash=$(${CKSUM} < config46.jag | cut -d' ' -f1)
printf "got checksum %s\n" "${hash}"
target='304ff7147a21119c4e47f913f5d27d65de8222d69d2f3317dff569d8a4061e94'
if [ "${hash}" != "${target}" ]; then
	printf "WARNING: config46.jag's checksum is wrong\n" >/dev/stderr
	printf "should be %s\n" "${target}" >/dev/stderr
	printf "delete and redownload files?\n" >/dev/stderr
	exit 1
fi

if ! [ -e maps27.jag ]; then
	printf "fetching maps27.jag\n"
	get_url https://web.archive.org/web/20020108023203/http://209.120.137.226:80/client/maps27.jag
fi
printf "verifying maps27.jag\n"
hash=$(${CKSUM} < maps27.jag | cut -d' ' -f1)
printf "got checksum %s\n" "${hash}"
target='3061da9b367ce0f6a21c94e7c74a016df49eed2ed274bae2a732d47a9655009b'
if [ "${hash}" != "${target}" ]; then
	printf "WARNING: maps27.jag's checksum is wrong\n" >/dev/stderr
	printf "should be %s\n" "${target}" >/dev/stderr
	printf "delete and redownload files?\n" >/dev/stderr
	exit 1
fi

printf "done\n"
exit 0
