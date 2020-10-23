use std::collections::HashSet;
use std::time::Instant;
use crate::graph::Graph;

fn calc_angle(p1: (f64, f64), p2: (f64, f64), p3: (f64, f64)) -> f64 {
    let v1 = (p1.0 - p2.0, p1.1 - p2.1);
    let v2 = (p3.0 - p2.0, p3.1 - p2.1);
    let cos_theta = (v1.0 * v2.0 + v1.1 * v2.1) /
        ((v1.0.powi(2) + v1.1.powi(2)).sqrt() * (v2.0.powi(2) + v2.1.powi(2)).sqrt());
    let theta = cos_theta.acos();

    theta
}

fn is_into_triangle(vt1: (f64, f64), vt2: (f64, f64), vt3: (f64, f64), pt: (f64, f64)) -> bool {
    let v1 = (vt2.0 - vt1.0, vt2.1 - vt1.1);
    let v2 = (vt3.0 - vt2.0, vt3.1 - vt2.1);
    let v3 = (vt1.0 - vt3.0, vt1.1 - vt3.1);
    let v1p = (pt.0 - vt1.0, pt.1 - vt1.1);
    let v2p = (pt.0 - vt2.0, pt.1 - vt2.1);
    let v3p = (pt.0 - vt3.0, pt.1 - vt3.1);
    // 「頂点 → 頂点 → 対象点」の外積 (の Z 値) を求める
    let outer_products_z = vec![
        v1.0 * v1p.1 - v1.1 * v1p.0,
        v2.0 * v2p.1 - v2.1 * v2p.0,
        v3.0 * v3p.1 - v3.1 * v3p.0,
    ];
    // これらが全て正、または全て負であるなら、 pt は 3頂点 vt* の内部に含まれる
    outer_products_z.iter().all(|&z| z >= 0.0) || outer_products_z.iter().all(|&z| z < 0.0)
}

fn sorted_by_polar_angle(points: &Vec<(f64, f64)>) -> Vec<(f64, f64)> {
    let (highest_v_idx, &highest_v_pt) = points.iter().enumerate()
        .min_by(|&a, &b| (a.1).1.partial_cmp(&(b.1).1).unwrap()).unwrap();
    let mut result: Vec<_> = points.iter().cloned().collect();
    result.remove(highest_v_idx);
    result.sort_by(|&a, &b|
        calc_angle((-20.0, highest_v_pt.1), highest_v_pt, a).partial_cmp(
            &calc_angle((-20.0, highest_v_pt.1), highest_v_pt, b)
        ).unwrap());
    result.insert(0, highest_v_pt);
    result
}

pub fn convex_hull_simple(graph: &mut Graph) {
    let now = Instant::now();

    let mut excludes = HashSet::<usize>::with_capacity(graph.points.len());
    for (i1, &p1) in graph.points.iter().enumerate() {
        for (i2, &p2) in graph.points.iter().enumerate() {
            if i2 == i1 { continue; }
            for (i3, &p3) in graph.points.iter().enumerate() {
                if [i1, i2].iter().any(|&i| i3 == i) { continue; }
                for (io, &po) in graph.points.iter().enumerate() {
                    if [i1, i2, i3].iter().any(|&i| io == i) || excludes.contains(&io) { continue; }
                    if is_into_triangle(p1, p2, p3, po) { excludes.insert(io); }
                }
            }
        }
    }
    let polygon = graph.points.iter().enumerate()
        .filter(|x| !excludes.contains(&x.0)).map(|x| *x.1).collect();
    let polygon = sorted_by_polar_angle(&polygon);

    let dur = now.elapsed();

    graph.polygon = polygon;
    graph.summary = format!("素朴解 - 処理時間: {}.{:03}秒", dur.as_secs(), dur.subsec_millis());
}
