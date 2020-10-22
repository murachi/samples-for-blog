pub mod graph;
pub mod convex_hull;

use crate::graph::Graph;

fn main() {
    let mut gr = Graph::new(20_usize);
    convex_hull::convex_hull_simple(&mut gr);
    gr.output_svg("out.svg");
}
