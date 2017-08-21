set $MAX_ADDR = 0x00600000

define my_ascii
  if $argc != 1
    printf "ERROR: my_ascii"
  else
    set $tmp = *(unsigned char *)($arg0)
    if ($tmp < 0x20 || $tmp > 0x7E)
      printf "."
    else
      printf "%c", $tmp
    end
  end
end

define my_hex
  if $argc != 1
    printf "ERROR: my_hex"
  else
    printf "%02X%02X%02X%02X ", \
      *(unsigned char*)($arg0 + 3), *(unsigned char*)($arg0 + 2),     \
      *(unsigned char*)($arg0 + 1), *(unsigned char*)($arg0 + 0)
  end
end

define hexdump
  if $argc != 2
   printf "ERROR: hexdump"
  else
    if ((*(unsigned char*)($arg0 + 0) == (unsigned char)($arg1 >>  0)))
      if ((*(unsigned char*)($arg0 + 1) == (unsigned char)($arg1 >>  8)))
        if ((*(unsigned char*)($arg0 + 2) == (unsigned char)($arg1 >> 16)))
          if ((*(unsigned char*)($arg0 + 3) == (unsigned char)($arg1 >> 24)))
            printf "%08X : ", $arg0
            my_hex $arg0
            my_ascii $arg0+0x3
            my_ascii $arg0+0x2
            my_ascii $arg0+0x1
            my_ascii $arg0+0x0
            printf "\n"
          end
        end
      end
    end
  end
end

define search_memloc
  set $max_addr = $MAX_ADDR
  set $counter = 0
  if $argc != 2
    help search_memloc
  else
    while (($arg0 + $counter) <= $max_addr)
      set $addr = $arg0 + $counter
      hexdump $addr $arg1
      set $counter = $counter + 0x20
    end
  end
end
document search_memloc
Search a kernel memory location that points to PATTERN.
Usage: search_memloc ADDRESS PATTERN
ADDRESS - address to start the search
PATTERN - pattern to search for
end