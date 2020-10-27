use std::path::Path;
use svg::Document;
use svg::node::element::{Polygon, Circle, Group, Text};
use svg::node::Text as CData;
use rand::prelude::*;

/// 凸包を描くグラフの描画領域。
/// タプルの各値 `(x, y, width, height)` は、領域の左上隅の座標値と、
/// 領域の幅・高さを表すものである。
const VIEWBOX: (f64, f64, f64, f64) = (0.0, 0.0, 100.0, 100.0);
/// SVG 出力時の上下左右のマージン。
const VIEWBOX_MARGIN: f64 = 20.0;

/// 凸包を描くグラフの情報を表す構造体。
pub struct Graph {
    /// 点集合。
    pub points: Vec<(f64, f64)>,
    /// 凸包を結ぶ多角形の頂点座標列。
    pub polygon: Vec<(f64, f64)>,
    /// SVG 上に表示するサマリー。算出方法、計算時間等を記載。
    pub summary: String,
}

impl Graph {
    /// コンストラクタ。
    /// # 引数
    /// - `point_num` ... (in) 生成する点集合の点の数。
    /// # 戻り値
    /// 新しく生成した [`Graph`](struct.Graph.html) 構造体。
    /// `graph.points` に `point_num` 個のランダムの点座標が格納される。
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

    /// 凸包グラフを SVG ファイルに出力する。
    /// # 引数
    /// - `file` ... (in) 出力するファイルのパス名。
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
                .set("x", VIEWBOX.0 + VIEWBOX.2).set("y", VIEWBOX.1 + VIEWBOX.3 + 3.0)
                .set("text-anchor", "end")
                .add(CData::new(self.summary.clone())));

        svg::save(file, &doc).unwrap();
    }
}
