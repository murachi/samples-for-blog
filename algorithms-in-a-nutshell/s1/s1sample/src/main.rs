pub mod graph;
pub mod convex_hull;

use crate::graph::Graph;

fn main() {
    let mut gr = Graph::new(50);
    convex_hull::convex_hull_simple(&mut gr);
    gr.output_svg("simple.svg");
    convex_hull::convex_hull_greedy(&mut gr);
    gr.output_svg("greedy.svg");
}
