$palette = %w{
  ? CLR_MAG
  # CLR_BLACK
  . CLR_WHITE
  B CLR_BLUE
  g RGB15(8,8,8)
  R CLR_RED
}.each_slice(2).to_a

def load_pal_bg
  $palette.each_with_index.map do |ary, i|
    char, color = ary
    "pal_bg_mem[#{i}] = #{color};"
  end.join("\n")
end

def load_tile(char_block, tile_index, tile)
  tile.each_with_index.map do |line, i|
    $palette.each_with_index do |ary, j|
      k = ary[0]
      v = j.to_s(16)
      line = line.gsub(k, v)
    end
    "tile_mem[#{char_block}][#{tile_index}].data[#{i}] = 0x#{line.reverse};"
  end.join("\n")
end

$outside = %w{
  BBBBBBBB
  BBBBBBBB
  BBBBBBBB
  BBBBBBBB
  BBBBBBBB
  BBBBBBBB
  BBBBBBBB
  BBBBBBBB
}

$bottom_edge = %w{
  ########
  BBBBBBBB
  BBBBBBBB
  BBBBBBBB
  BBBBBBBB
  BBBBBBBB
  BBBBBBBB
  BBBBBBBB
}

$right_edge = %w{
  #BBBBBBB
  #BBBBBBB
  #BBBBBBB
  #BBBBBBB
  #BBBBBBB
  #BBBBBBB
  #BBBBBBB
  #BBBBBBB
}

$bottom_right_corner = %w{
  #BBBBBBB
  BBBBBBBB
  BBBBBBBB
  BBBBBBBB
  BBBBBBBB
  BBBBBBBB
  BBBBBBBB
  BBBBBBBB
}

$white_tile = %w{
  ########
  #.......
  #.......
  #.......
  #.......
  #.......
  #.......
  #.......
}

$dot_tile = %w{
  ########
  #.......
  #.......
  #...g...
  #..g#g..
  #...g...
  #.......
  #.......
}

$black_tile = %w{
  ########
  #ggggggg
  #ggggggg
  #ggggggg
  #ggggggg
  #ggggggg
  #ggggggg
  #ggggggg
}

$num_tiles = [
  %w{
    ########
    #.......
    #.#####.
    #.#...#.
    #.#...#.
    #.#...#.
    #.#####.
    #.......
  },
  %w{
    ########
    #.......
    #...#...
    #...#...
    #...#...
    #...#...
    #...#...
    #.......
  },
  %w{
    ########
    #.......
    #.#####.
    #.....#.
    #.#####.
    #.#.....
    #.#####.
    #.......
  },
  %w{
    ########
    #.......
    #.#####.
    #.....#.
    #.#####.
    #.....#.
    #.#####.
    #.......
  },
  %w{
    ########
    #.......
    #.#...#.
    #.#...#.
    #.#####.
    #.....#.
    #.....#.
    #.......
  },
  %w{
    ########
    #.......
    #.#####.
    #.#.....
    #.#####.
    #.....#.
    #.#####.
    #.......
  },
  %w{
    ########
    #.......
    #.#####.
    #.#.....
    #.#####.
    #.#...#.
    #.#####.
    #.......
  },
  %w{
    ########
    #.......
    #.#####.
    #.....#.
    #.....#.
    #.....#.
    #.....#.
    #.......
  },
  %w{
    ########
    #.......
    #.#####.
    #.#...#.
    #.#####.
    #.#...#.
    #.#####.
    #.......
  },
  %w{
    ########
    #.......
    #.#####.
    #.#...#.
    #.#####.
    #.....#.
    #.#####.
    #.......
  },
  %w{
    ########
    #.......
    #.#.###.
    #.#.#.#.
    #.#.#.#.
    #.#.#.#.
    #.#.###.
    #.......
  },
  %w{
    ########
    #.......
    #..#.#..
    #..#.#..
    #..#.#..
    #..#.#..
    #..#.#..
    #.......
  },
  %w{
    ########
    #.......
    #.#.###.
    #.#...#.
    #.#.###.
    #.#.#...
    #.#.###.
    #.......
  },
  %w{
    ########
    #.......
    #.#.###.
    #.#...#.
    #.#.###.
    #.#...#.
    #.#.###.
    #.......
  },
  %w{
    ########
    #.......
    #.#.#.#.
    #.#.#.#.
    #.#.###.
    #.#...#.
    #.#...#.
    #.......
  },
  %w{
    ########
    #.......
    #.#.###.
    #.#.#...
    #.#.###.
    #.#...#.
    #.#.###.
    #.......
  },
  %w{
    ########
    #.......
    #.#.###.
    #.#.#...
    #.#.###.
    #.#.#.#.
    #.#.###.
    #.......
  },
  %w{
    ########
    #.......
    #.#.###.
    #.#...#.
    #.#...#.
    #.#...#.
    #.#...#.
    #.......
  },
  %w{
    ########
    #.......
    #.#.###.
    #.#.#.#.
    #.#.###.
    #.#.#.#.
    #.#.###.
    #.......
  },
  %w{
    ########
    #.......
    #.#.###.
    #.#.#.#.
    #.#.###.
    #.#...#.
    #.#.###.
    #.......
  },
]

$cursor = %w{
  ????????
  ?RR???RR
  ?R?????R
  ????????
  ????????
  ????????
  ?R?????R
  ?RR???RR
}

def layer(back, front)
  (0..7).map do |r|
    (0..7).map do |c|
      if front[r][c] == '?'
        back[r][c]
      else
        front[r][c]
      end
    end.join('')
  end
end
