use std::collections::HashSet;
use std::cmp::Ordering;
use std::time::Instant;
use crate::graph::Graph;

/// 回転の方向を示す列挙型。
///
/// X軸は右方向、 Y軸は下方向に伸びる座標系を前提としている。
#[derive(PartialEq)]
enum RotationDirection {
    /// 時計回り
    Clockwise,
    /// 反時計回り
    Anticlockwise,
    /// 同一直線上
    Straight,
}

/// 3点の角度を求める。
/// # 引数
/// いずれも `f64` 型 2つのタプル `(x, y)` を指定する。
/// - `p1` ... (in) 点a
/// - `p2` ... (in) 点b
/// - `p3` ... (in) 点c
/// # 戻り値
/// ∠abc の内積角度をラジアン単位で返す。範囲は区間 `[0, π]`。
fn calc_angle(p1: (f64, f64), p2: (f64, f64), p3: (f64, f64)) -> f64 {
    let v1 = (p1.0 - p2.0, p1.1 - p2.1);
    let v2 = (p3.0 - p2.0, p3.1 - p2.1);
    let cos_theta = (v1.0 * v2.0 + v1.1 * v2.1) /
        ((v1.0.powi(2) + v1.1.powi(2)).sqrt() * (v2.0.powi(2) + v2.1.powi(2)).sqrt());
    let theta = cos_theta.acos();

    theta
}

/// 3頂点を順番に描く場合の回転方向を求める。
/// # 引数
/// いずれも `f64` 型 2つのタプル `(x, y)` を指定する。
/// - `vt1` ... (in) 頂点A
/// - `vt2` ... (in) 頂点B
/// - `vt3` ... (in) 頂点C
/// # 戻り値
/// 頂点 A, B, C の順に通る経路の回転方向を示す
/// [`RotationDirection`](enum.RotationDirection.html) 値を返す。
fn calc_rotation_direction(vt1: (f64, f64), vt2: (f64, f64), vt3: (f64, f64))
-> RotationDirection {
    let v1 = (vt2.0 - vt1.0, vt2.1 - vt1.1);
    let v2 = (vt3.0 - vt2.0, vt3.1 - vt2.1);
    let outer_product_z = v1.0 * v2.1 - v1.1 * v2.0;
    match outer_product_z.partial_cmp(&0.0).unwrap() {
        Ordering::Greater => RotationDirection::Clockwise,
        Ordering::Less => RotationDirection::Anticlockwise,
        Ordering::Equal => RotationDirection::Straight,
    }
}

/// 点が 3頂点からなる三角形に含まれるかをテストする。
/// # 引数
/// いずれも `f64` 型 2つのタプル `(x, y)` を指定する。
/// - `vt1` ... (in) 頂点A
/// - `vt2` ... (in) 頂点B
/// - `vt3` ... (in) 頂点C
/// - `pt` ... (in) 調査対象の点P
/// # 戻り値
/// 点P が⊿ABC の内部に含まれる点であるなら `true` を、それ以外の場合は `false` を返す。
///
/// 点P が⊿ABC の辺の上に存在する場合は、⊿ABC の内部に含まれないものとして扱う。
fn is_into_triangle(vt1: (f64, f64), vt2: (f64, f64), vt3: (f64, f64), pt: (f64, f64)) -> bool {
    let rds = [
        calc_rotation_direction(vt1, vt2, pt),
        calc_rotation_direction(vt2, vt3, pt),
        calc_rotation_direction(vt3, vt1, pt),
    ];
    // 全て時計回りか、または全て反時計回りであるなら、 pt は 3頂点 vt* の内部に含まれる
    rds.iter().all(|rd| *rd == RotationDirection::Clockwise) ||
    rds.iter().all(|rd| *rd == RotationDirection::Anticlockwise)
}

/// 座標値のリストを極角が小さい順にソートしたものを生成する。
/// # 引数
/// - `points` ... (in) 点座標のベクタ
/// # 戻り値
/// ソートされた点座標のベクタを返す。
/// # 解説
/// 座標は以下のとおりにソートされる。
/// 1. 先頭には Y座標値が最も小さい点を選択する。この点を仮に A とする。
/// 1. 点A の座標値 (x, y) に対して、座標値 (-20.0, y) となる点 O を取る
///    (この点は戻り地のベクタには含まれない)。
/// 1. 点A を除く調査対象の点P について、∠OAP の内積角度が小さい順にソートし、
///    点A に続けて並べる。
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

/// 凸包を求める (素朴解)
/// # 引数
/// - `graph` ... (in/out) グラフオブジェクト
/// # 解説
/// `graph.points` の点集合から全ての三角形を形成する 3頂点を逐次調査し、
/// いずれかの三角形に含まれる点を除外した残りの点を縫合して凸包を求め、
/// `graph.polygon` に格納する。
///
/// 計算回数 N^4 となるため、とても効率が悪い。
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

/// 凸包を求める (貪欲法)
/// # 引数
/// - `graph` ... (in/out) グラフオブジェクト
/// # 解説
/// 最初に `graph.points` を極角でソートし、ソートした順に線で結んだときに
/// 頂点の曲がる方向が時計回りになってしまうような点を除外しながら結んでいく
/// 方法で凸包を求める。
///
/// 出来上がった凸包は、 Y座標値が最も小さい頂点を先頭に、反時計回り方向で
/// 辿るような順序で `graph.polygon` に格納される。
///
/// 計算回数は最悪でもほぼ N^2/2 となる。
pub fn convex_hull_greedy(graph: &mut Graph) {
    let now = Instant::now();

    let points = sorted_by_polar_angle(&graph.points);
    // points は先頭 pt0 (x0, y0) に対する pt (x, y) について
    // (-20.0, y0) -> (x0, y0) -> (x, y) の内積角度の小さい順にソートしている。
    // これを辿って描かれる凸包は、各頂点が常に反時計回りで描かれることになる。
    let mut polygon = Vec::<(f64, f64)>::with_capacity(points.len());
    for pt in points {
        if polygon.len() >= 2 {
            while calc_rotation_direction(
                polygon[polygon.len() - 2],
                polygon[polygon.len() - 1],
                pt) == RotationDirection::Clockwise
            {
                polygon.pop();
            }
        }
        polygon.push(pt);
    }
    let dur = now.elapsed();

    graph.polygon = polygon;
    graph.summary = format!("貪欲法 - 処理時間: {}.{:03}秒", dur.as_secs(), dur.subsec_millis());
}
