pub mod graph;

use crate::graph::*;

fn main() {
    let gr = Graph::new(20_usize);
    gr.output_svg("out.svg");
}
