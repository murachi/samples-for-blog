use std::path::Path;
use svg::Document;
use svg::node::element::{Polygon, Circle, Group};
use rand::prelude::*;

const HORIZON_RANGE: (f64, f64) = (0f64, 100f64);
const VERTICAL_RANGE: (f64, f64) = (0f64, 100f64);

pub struct Graph {
  points: Vec<(f64, f64)>,
  polygon: Vec<(f64, f64)>,
}

impl Graph {
  pub fn new(point_num: usize) -> Graph {
      let mut points = Vec::<(f64, f64)>::with_capacity(point_num);
      let mut rng = rand::thread_rng();
      for _ in 0..point_num {
          points.push((
              rng.gen::<f64>() * (HORIZON_RANGE.1 - HORIZON_RANGE.0) + HORIZON_RANGE.0,
              rng.gen::<f64>() * (VERTICAL_RANGE.1 - VERTICAL_RANGE.0) + VERTICAL_RANGE.0));
      }
      Graph {
          points: points,
          polygon: vec![],
      }
  }

  pub fn output_svg<T: AsRef<Path>>(&self, file: T) {
      let mut doc = Document::new()
        .set("viewBox", (
            HORIZON_RANGE.0 - 20f64, VERTICAL_RANGE.0 - 20f64,
            HORIZON_RANGE.1 + 20f64, VERTICAL_RANGE.1 + 20f64))
        .set("width", "18cm")
        .set("height", "18cm");
      let mut dot_grp = Group::new()
        .set("fill", "#008");
      for pt in &self.points {
          dot_grp = dot_grp.add(Circle::new().set("cx", pt.0).set("cy", pt.1).set("r", 1f64));
      }
      let poly_grp = Group::new()
        .set("stroke", "#aaa")
        .set("stroke-width", "2")
        .add(Polygon::new().set("points", self.polygon.clone()));
      doc = doc.add(poly_grp).add(dot_grp);

      svg::save(file, &doc).unwrap();
  }
}
