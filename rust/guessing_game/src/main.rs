extern crate rand;

use std::io;
use std::cmp::Ordering;
use rand::Rng;

fn main() {
    println!("数あてゲームです。");

    let secret_num = rand::thread_rng().gen_range(1, 101);

    //println!("(((カンニング… 答えは {} です…)))", secret_num);

    loop {
        println!("私が考えている数を予想してください…");

        let mut guess = String::new();

        io::stdin().read_line(&mut guess)
        .expect("ユーザーからの入力に失敗");

        let guess: i32 = match guess.trim().parse() {
            Ok(num) => num,
            Err(_) => continue,
        };

        println!("あなたの予想は {} です。", guess);

        match guess.cmp(&secret_num) {
            Ordering::Less => println!("が、それだと小さいです。"),
            Ordering::Greater => println!("が、それだと大きいです。"),
            Ordering::Equal => {
                println!("よくぞ当てました!!");
                break;
            },
        }
    }
}
