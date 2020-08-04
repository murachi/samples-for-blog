fn main() {
    let x = String::from("Hello");
    let len = string_length(&x);
    println!("len = {}", len);
    println!("x = {}", x);

    println!("test(-50) -> {}", test(-50));
    println!("test(200) -> {}", test(200));
    println!("test(42) -> {}", test(42));
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
