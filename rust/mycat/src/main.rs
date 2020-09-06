use std::env;
//use std::fs;
use std::fs::File;
use std::io::{Read, BufRead, BufReader, Write, BufWriter, stdout};
//use std::process::exit;
use std::error::Error;
use std::fmt;

#[derive(Debug)]
struct CmdOptionError<'a> {
    msg: &'a str,
}
impl fmt::Display for CmdOptionError<'_> {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "コマンドオプションのエラー ({})", self.msg)
    }
}
impl Error for CmdOptionError<'_> {}

struct CmdOption<'a> {
    infile: &'a str,
    outfile: Option<&'a str>,
}
impl CmdOption<'_> {
    fn new<'a, 'b>(args: &'a Vec<String>) -> Result<CmdOption<'a>, CmdOptionError<'b>> {
        match args.len() {
            0..=1 => {
                Err(CmdOptionError{
                    msg: "ファイル名を指定してください (usage: mycat infile [outfile])",
                })
            },
            2..=3 => {
                Ok(CmdOption {
                    infile: args[1].as_str(),
                    outfile: if args.len() == 3 { Some(args[2].as_str()) } else { None },
                })
            },
            _ => {
                Err(CmdOptionError{
                    msg: "引数が多すぎます",
                })
            },
        }
    }
}

fn main() -> Result<(), Box<dyn Error>>{
    let args: Vec<_> = env::args().collect();
    let cmd_opt = CmdOption::new(&args)?;

    //if let Ok(text) = fs::read_to_string(&args[1]) {
    //    println!("{}", text);
    //}
    //else {
    //    eprintln!("ファイル {} を開けません。", args[1]);
    //    exit(1);
    //}
    //match fs::read_to_string(&args[1]) {
    //    Ok(text) => { println!("{}", text); },
    //    _ => { eprintln!("ファイル {} を開けません。", args[1]); exit(1); },
    //}

    let fin = File::open(cmd_opt.infile)?;
    //    .expect(&format!("ファイル {} を開けません。", args[1]));
    if let Some(name) = cmd_opt.outfile {
        read_and_write(fin, File::create(name)?)?;
    }
    else {
        read_and_write(fin, stdout())?;
    }
    //for iter in BufReader::new(file).lines() {
    //    if let Ok(line) = iter {
    //        println!("{}", line);
    //    }
    //}
    Ok(())
}

fn read_and_write<R: Read, T: Write>(fin: R, target: T) -> std::io::Result<()> {
    let mut target = BufWriter::new(target);
    for line in BufReader::new(fin).lines() {
        writeln!(target, "{}", line?)?;
    }
    Ok(())
}
