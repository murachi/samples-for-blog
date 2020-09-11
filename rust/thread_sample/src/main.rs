use std::thread;
use std::time::Duration;
use futures::future::Future;
use futures::executor::ThreadPool;
use std::io::Read;

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

    let pool = ThreadPool::new().expect("create thread pool failed.");

    println!("please wait.");
    pool.spawn_ok(make_task(1, 1_000_u64));
    pool.spawn_ok(make_task(2, 2_000_u64));
    pool.spawn_ok(make_task(3, 1_500_u64));
    //h1.join().expect("thread h1 join failed.");
    //h2.join().expect("thread h2 join failed.");
    //h3.join().expect("thread h3 join failed.");
    println!("press any key.");
    std::io::stdin().read(&mut [0]).unwrap();
    println!("program end.");
}
