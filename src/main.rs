use std::{env, fs::File, io::Read, path::Path, process::exit};

use lexer::Lexer;

mod lexer;

fn main() {
  let cli_args: Vec<String> = env::args().collect();

  let input_path = match &cli_args[..] {
    [_cmd] => Some("input.hy"),
    [_cmd, input_file] => Some(input_file.as_str()),
    [_cmd, ..] => {
      eprintln!("Usage: {} <input_file>", _cmd);
      exit(1);
    }
    _ => {
      eprintln!("Usage: hyc <input_file>");
      exit(1);
    }
  }
  .unwrap();

  let mut input_file = File::open(&Path::new(input_path)).unwrap();
  let mut source = String::new();
  input_file.read_to_string(&mut source).unwrap();

  let lexer = Lexer::new(&source);
  let tokens = lexer.collect::<Vec<_>>();

  println!("All Tokens:");
  for t in tokens.iter() {
    println!("\t{:?}", t);
  }
}
