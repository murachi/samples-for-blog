use svg::Document;
use svg::node::element::{Polygon, Circle, Group}
use rand::prelude::*;

pub struct Graph {
  points: Vec<f64>,
  polygon: Vec<f64>,
}

impl Graph {
  pub fn new(usize point_num) -> Graph {
      let mut points = Vec::<f64>::with_capacity(point_num);
      let mut rng = rand::thread_rng();
      for i in (0..point_num) {
          points[i] = rng.gen();
      }
      Graph {
          points: points,
          polygon: vec![],
      }
  }

  pub fn output_svn() {}
}
