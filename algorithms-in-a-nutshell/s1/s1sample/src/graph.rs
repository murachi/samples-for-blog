use std::path::Path;
use svg::Document;
use svg::node::element::{Polygon, Circle, Group, Text};
use svg::node::Text as CData;
use rand::prelude::*;

const VIEWBOX: (f64, f64, f64, f64) = (0.0, 0.0, 100.0, 100.0);
const VIEWBOX_MARGIN: f64 = 20.0;

pub struct Graph {
  pub points: Vec<(f64, f64)>,
  pub polygon: Vec<(f64, f64)>,
  pub summary: String,
}

impl Graph {
  pub fn new(point_num: usize) -> Graph {
      let mut points = Vec::<(f64, f64)>::with_capacity(point_num);
      let mut rng = rand::thread_rng();
      for _ in 0..point_num {
          points.push((
              rng.gen::<f64>() * VIEWBOX.2 + VIEWBOX.0,
              rng.gen::<f64>() * VIEWBOX.3 + VIEWBOX.1));
      }
      Graph {
          points: points,
          polygon: vec![],
          summary: "".to_string(),
      }
  }

  pub fn output_svg<T: AsRef<Path>>(&self, file: T) {
      let mut doc = Document::new()
        .set("viewBox", (
            VIEWBOX.0 - VIEWBOX_MARGIN,
            VIEWBOX.1 - VIEWBOX_MARGIN,
            VIEWBOX.0 + VIEWBOX.2 + VIEWBOX_MARGIN * 2.0,
            VIEWBOX.1 + VIEWBOX.3 + VIEWBOX_MARGIN * 2.0))
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
        .set("fill", "none")
        .add(Polygon::new().set("points", self.polygon.clone()));
      doc = doc.add(poly_grp).add(dot_grp)
        .add(Text::new()
            .set("font-family", "sans-serif").set("fill", "#444").set("font-size", 4.0)
            .set("x", VIEWBOX.2).set("y", VIEWBOX.3 + 3.0).set("text-anchor", "end")
            .add(CData::new(self.summary.clone())));

      svg::save(file, &doc).unwrap();
  }
}
