use std::thread;
use std::time::Duration;
use futures::future::{self, Future};
use futures::executor;

fn make_task(num: i32, dur: u64) -> impl Future<Output = ()> {
    async move {
        let mut dur_total = 0_u64;
        for i in 0..10 {
            println!("thread #{} count {} ({}ms).", num, i, dur_total);
            thread::sleep(Duration::from_millis(dur));
            dur_total += dur;
        }
    }
}

fn main() {
    //let h1 = thread::spawn(make_task(1, 1_000_u64));
    //let h2 = thread::spawn(make_task(2, 2_000_u64));
    //let h3 = thread::spawn(make_task(3, 1_500_u64));

    //let h1 = async {
    //    let mut dur_total = 0_u64;
    //    for i in 0..10 {
    //        println!("thread #1 count {} ({}ms).", i, dur_total);
    //        thread::sleep(Duration::from_millis(1_000_u64));
    //        dur_total += 1_000_u64;
    //    }
    //};
    //let h2 = async {
    //    let mut dur_total = 0_u64;
    //    for i in 0..10 {
    //        println!("thread #2 count {} ({}ms).", i, dur_total);
    //        thread::sleep(Duration::from_millis(1_500_u64));
    //        dur_total += 1_500_u64;
    //    }
    //};

    println!("please wait.");
    let joined_task = future::join_all(vec![
        make_task(1, 1_000_u64),
        make_task(2, 2_000_u64),
        make_task(3, 1_500_u64)]);
    //let joined_task = future::join3(make_task(1, 1_000_u64), make_task(2, 2_000_u64), make_task(3, 1_500_u64));
    //let joined_task = future::join(h1, h2);
    executor::block_on(joined_task);
    //h1.join().expect("thread h1 join failed.");
    //h2.join().expect("thread h2 join failed.");
    //h3.join().expect("thread h3 join failed.");
    println!("program end.");
}
