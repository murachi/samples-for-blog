//use std::num::ParseFloatError;
use std::os::raw::c_char;
use std::ffi::{CStr, CString};
use std::error::Error;
use std::cmp::Ordering;

extern {
    fn puts(s: *const c_char);
    fn strlen(s: *const c_char) -> usize;
}

#[link(name="hello", kind="static")]
extern {
    fn hello();
    fn static_str() -> *const c_char;
    fn dynamic_str() -> *mut c_char;
}

#[allow(dead_code)]
enum LANG {
    Japanese,
    English,
    Chinese,
    Franch,
    Germany,
}

fn main() -> Result<(), Box<dyn Error>> {
    let x = String::from("Hello");
    let len = string_length(&x);
    println!("len = {}", len);
    println!("x = {}", x);

    println!("test(-50) -> {}", test(-50));
    println!("test(200) -> {}", test(200));
    println!("test(42) -> {}", test(42));

    for i in (1..4).rev() {
        println!("{}!", i);
    }
    println!("発射!!");

    let bits: u8 = 0b00110101;
    println!("not {:08b} == {:08b}", bits, !bits);

    let mut tx = String::from("Yahoo!");
    println!("tx = {}", tx);
    str_param_modify(&mut tx);
    println!("tx (modified) = {}", tx);
    let tx = str_param_modify_2(&tx);
    println!("tx (modified(2)) = {}", tx);

    let primes = vec![2, 3, 5, 7, 11, 13, 17, 19];
    let sum_of_primes = vec_param(&primes);
    println!("sum is {}", sum_of_primes);

    let mut range = make_range(10);
    print_vec("range", &range);
    //for n in &range {
    //    print!("{} ", n);
    //}
    //println!("");

    vec_modify(&mut range);
    print_vec("range (modified)", &range);
    //for n in &range {
    //    print!("{} ", n);
    //}
    //println!("");

    let lang = LANG::Germany;
    let lang = match lang {
        LANG::Japanese => "日本語",
        LANG::English => "英語",
        LANG::Chinese => "中国語",
        LANG::Franch => "フランス語",
        LANG::Germany => "ドイツ語",
        //_ => "日本語以外",
    };
    println!("lang is {}", lang);

    let x = Some(10);
    let v = match x {
        Some(n) => n,
        None => -1,
    };
    println!("v is {}", v);
    if let Some(n) = x {
        println!("x has {}", n);
    }

    let age = 42;
    println!("{}歳は{}", age, get_age_name(&age));

    force_cast_array();

    vector_sample1();

    iter_sample1();
    iter_sample2();

    let texts = ["100", "xxx", "3.141592653589793236", "0xFB386"];
    for r in texts.iter().map(|t| half_number(t)) {
        match r {
            Ok(n) => println!("Ok: {}", n),
            Err(ex) => println!("Error: {:?}", ex),
        }
    }

    unsafe {
        hello();
    }

    let null_terminated_text = CString::new("Rust の世界へようこそ…。")?;
    unsafe {
        puts(null_terminated_text.as_ptr());
    }
    let n = unsafe {
        strlen(null_terminated_text.as_ptr())
    };
    println!("s.len is {}", n);

    let a = [2, 3, 5, 6, 8, 9, 11, 12];
    let ie3 = a.iter().find(|&x: &&i32| *x == 3);
    let igt10 = a.iter().find(|&&x: &&i32| x > 10);
    let ile1 = a.iter().find(|&&x| x <= 1);
    if let Some(&n) = ie3 {
        print_num(n, "ie3");
    }
    if let Some(n) = igt10 {
        print_num(*n, "igt10");
    }
    println!("ile1 = {:?}", ile1);

    let b = a.iter().filter(|&&x| x % 2 == 1);
    for it in b {
        println!("it is {}", it);
    }

    let c: Vec<_> = a.iter().map(|n: &i32| n * 2).collect();
    println!("c is {:?}", c);

    if let Some(o) = 1f64.partial_cmp(&2f64) {
        match o {
            Ordering::Less => println!("1 < 2"),
            _ => println!("!!?"),
        }
    }
    else { println!("!!!"); }

    let text = get_c_text();
    println!("static_str() -> '{}'", text);
    let text = unsafe { CString::from_raw(dynamic_str()) }.into_string()?;
    println!("dynamic_str() -> '{}'", text);

    Ok(())
}

fn get_c_text<'a>() -> &'a str {
    unsafe { CStr::from_ptr(static_str()) }.to_str().unwrap()
}

fn print_num(n: i32, name: &str) {
    println!("{} = {}", name, n);
}

fn half_number(s: &str) -> Result<f64, String> {
    //s.parse::<f64>().map(|n| n / 2.0)
    match s.parse::<f64>() {
        Ok(n) => Ok(n / 2.0),
        Err(_) => Err(format!("'{}' を実数値に変換できません", s)),
    }
}

fn iter_sample2() {
    let v = vec![1, 2, 3, 4, 5];
    for n in v.iter().map(|x| x * 2) {
        print!("{} ", n);
    }
    println!("");
    for n in &v {
        print!("{} ", n);
    }
    println!("");
}

fn iter_sample1() {
    let primes = "2,3,5,7,11,13,17,19"
        .split(',')
        .map(|x| x.parse::<i32>().expect("isn't digit"))
        .collect::<Vec<_>>();

    print_vec("primes", &primes);
}

fn vector_sample1() {
    //let mut v: Vec<i32> = Vec::new();
    //let mut v = Vec::<i32>::new();
    let mut v = Vec::new();
    println!("v.len is {}", v.len());

    v.push(1000);
    v.push(2000);
    v.push(3900);
    v.push(7700);
    v.push(8000);

    println!("v.len is {}", v.len());

    println!("v.first is {:?}", v.first());
    println!("v.last is {:?}", v.last());
    println!("v.get(1) is {:?}", v.get(1));
    println!("v.get(10) is {:?}", v.get(10));

    while v.len() > 0 {
        println!("poped {:?} from v", v.pop());
    }
    println!("v.len is {}", v.len());
}

fn force_cast_array() {
    let a = [1u8, 2u8, 3u8, 4u8];
    unsafe {
        let b = std::mem::transmute::<[u8; 4], u32>(a);
        println!("[{:X}, {:X}, {:X}, {:X}] -> {:X}", a[0], a[1], a[2], a[3], b);
    }
    let a: u32 = 0x11223344;
    unsafe {
        let b = std::mem::transmute::<u32, [u8; 4]>(a);
        println!("{:X} -> [{:X}, {:X}, {:X}, {:X}]", a, b[0], b[1], b[2], b[3]);
    }
}

fn get_age_name(age: &i32) -> &str {
    match *age {
        0..=1 => "乳児",
        2..=4 => "幼児",
        5..=15 => "少年",
        16..=29 => "青年",
        30..=39 => "壮年",
        40..=54 => "中年",
        55..=64 => "熟年",
        65..=74 => "高年・老年",
        75..=99 => "後期高齢者",
        100..=119 => "百寿者",
        _ => "仙人",
    }
}

fn vec_modify(v: &mut Vec<i32>) {
    for i in v {
        *i *= *i;
    }
}

fn print_vec(text: &str, v: &Vec<impl std::fmt::Display>) {
    print!("{}: ", text);
    for (i, n) in v.iter().enumerate() {
        print!("[{}]{} ", i, n);
    }
    println!("");
}

fn make_range(max: i32) -> Vec<i32> {
    let mut v = Vec::new();
    for i in 0..max {
        v.push(i);
    }
    v
}

fn vec_param(v: &Vec<i32>) -> i32 {
    let mut sum = 0;
    for n in v { sum += n; }
    sum
}

fn string_length(s: &String) -> usize {
    let length = s.len();
    length
}

fn test(x: i32) -> i32 {
    let ans =
        if x < 0 { 0 }
        else if x > 100 { 100 }
        else { x };

    ans
}

fn str_param_modify(s: &mut String) {
    *s = String::from("hello");
}

fn str_param_modify_2(s: &str) -> String {
    format!("hello {} world.", s)
}
