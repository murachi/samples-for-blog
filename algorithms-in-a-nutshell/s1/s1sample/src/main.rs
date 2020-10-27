/// 凸包グラフを収める構造体の実装。
pub mod graph;
/// 凸包の計算を行う関数群の実装。
pub mod convex_hull;

use crate::graph::Graph;

fn main() {
    let mut gr = Graph::new(50);
    convex_hull::convex_hull_simple(&mut gr);
    gr.output_svg("simple.svg");
    convex_hull::convex_hull_greedy(&mut gr);
    gr.output_svg("greedy.svg");
}
