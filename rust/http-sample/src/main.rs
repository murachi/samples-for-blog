use std::error::Error;
use reqwest::StatusCode;

#[tokio::main]
async fn main() -> Result<(), Box<dyn Error>> {
    let url = "https://tmpbox.harapeko.jp/1";
    //let url = "https://developer.harapeko.jp/trac/original/ideanote/login";
    println!("call {}", url);
    if let Ok(res) = reqwest::get(url).await {
        match res.status() {
            StatusCode::OK => {
                println!("responsed url -> {}", res.url());
                let body = res.text().await?;
                println!("response is \n{}", body);
            },
            StatusCode::NOT_FOUND => {
                println!("コンテンツは見つかりませんでした…。");
            },
            _ => {
                println!("何かしらのエラー? が発生 ({})", res.status().as_u16());
            },
        }
    }
    else {
        println!("Webサーバーが見つかりませんでした…");
    }
    Ok(())
}
