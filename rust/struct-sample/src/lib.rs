pub mod figure {
    pub struct Rectangle<T> {
        left: T,
        top: T,
        width: T,
        height: T,
    }
    impl<T: Copy> Rectangle<T> {
        pub fn new(pos: (T, T), size: (T, T)) -> Rectangle<T> {
            Rectangle {
                left: pos.0,
                top: pos.1,
                width: size.0,
                height: size.1,
            }
        }
    }

    pub struct Polygon<T> {
        vertexes: [(T, T); 3],
    }
    impl<T: Copy> Polygon<T> {
        pub fn new(vertexes: [(T, T); 3]) -> Polygon<T> {
            Polygon { vertexes: vertexes }
        }
        pub fn mod_vertex(&mut self, idx: usize, vert: (T, T)) {
            self.vertexes[idx] = vert;
        }
    }

    pub struct Circle<T> {
        center: (T, T),
        radius: T,
    }
    impl<T: Copy> Circle<T> {
        pub fn new(center: (T, T), radius: T) -> Circle<T> {
            Circle { center: center, radius: radius }
        }
    }

    pub trait CalcArea {
        fn calc_area(&self) -> f64;
    }

    impl<T: std::convert::Into<f64> + Copy> CalcArea for Rectangle<T> {
        fn calc_area(&self) -> f64 {
            self.width.into() * self.height.into()
        }
    }
    impl<T: std::convert::Into<f64> + Copy + PartialOrd> CalcArea for Polygon<T> {
        fn calc_area(&self) -> f64 {
            // Y座標が 2番目になる頂点を選択 ...(1)
            let c01 = self.vertexes[0].1 <= self.vertexes[1].1;
            let c12 = self.vertexes[1].1 <= self.vertexes[2].1;
            let c20 = self.vertexes[2].1 <= self.vertexes[0].1;
            let middle_n =
                if c01 && c12 { 1 }
                else if c12 && c20 { 2 }
                else { 0 };
            // それ以外の頂点 ...(2)
            let other_n = [(middle_n + 1) % 3, (middle_n + 2) % 3];
            // (2) の線分の Y座標が (1) と同じになる点 ...(3)
            let ratio =
                (self.vertexes[middle_n].1.into() - self.vertexes[other_n[1]].1.into()) /
                (self.vertexes[other_n[0]].1.into() - self.vertexes[other_n[1]].1.into());
            let div_pt = (
                // x2 + (x1 - x2) * rat = x2 * (1 - rat) + x1 * rat
                self.vertexes[other_n[1]].0.into() * (1.0 - ratio)
                    + self.vertexes[other_n[0]].0.into() * ratio,
                self.vertexes[middle_n].1.into());
            // (3) と (1) の X座標の差を底辺の長さ、 (2) の Y座標の差を高さとすれば
            // 面積が求められる
            let area = (self.vertexes[middle_n].0.into() - div_pt.0)
                * (self.vertexes[other_n[0]].1.into() - self.vertexes[other_n[1]].1.into())
                / 2.0;
            if area > 0.0 { area } else { -area }
        }
    }
    impl<T: std::convert::Into<f64> + Copy> CalcArea for Circle<T> {
        fn calc_area(&self) -> f64 {
            self.radius.into() * self.radius.into() * std::f64::consts::PI
        }
    }

}
