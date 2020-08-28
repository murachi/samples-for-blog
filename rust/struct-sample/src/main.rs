struct Person {
    id: i32,
    name: String,
    age: i32,
    addr: String,
}
impl Person {
    fn new(id_src: &mut i32, name: &str, age: i32, addr: &str) -> Person {
        let pa = Person {
            id: *id_src,
            name: name.to_string(),
            age: age,
            addr: addr.to_string(),
        };
        *id_src += 1;
        pa
    }

    fn print(&self, keep_privacy: bool) {
        println!("{}", self.to_str(keep_privacy));
    }

    fn to_str(&self, keep_privacy: bool) -> String {
        if keep_privacy {
            format!("{}: {}", self.id, self.name)
        }
        else {
            format!("{}: {} ({}) in {}", self.id, self.name, self.age, self.addr)
        }
    }

    fn add_age(&mut self, n: i32) {
        self.age += n;
    }
}

struct PersonFactory {
    person_id_src: i32,
}
impl PersonFactory {
    fn new() -> PersonFactory {
        PersonFactory {
            person_id_src: 100,
        }
    }

    fn gen_person(&mut self, name: &str, age: i32, addr: &str) -> Person {
        Person::new(&mut self.person_id_src, name, age, addr)
    }
}

struct Rectangle<T> {
    left: T,
    top: T,
    width: T,
    height: T,
}

struct Polygon<T> {
    vertexes: [(T, T); 3],
}

struct Circle<T> {
    center: (T, T),
    radius: T,
}

trait CalcArea {
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

fn main() {
    let mut pf = PersonFactory::new();
    let mut pa = pf.gen_person("Murayama Toshiyuki", 42, "Narashino Chiba, Japan");
    pa.print(true); // print_person(&pa);

    pa.name = "T.MURACHI".to_string();
    pa.add_age(9957);
    pa.addr = "Somewhere in the earth.".to_string();
    println!("!!!! {} !!!!", pa.to_str(false));    // pa.print(false);

    let mut people = vec![
        pa,
        pf.gen_person("だだぞう", 714, "秘密"),
        pf.gen_person("りりぞう", 4, "しーしゃんち"),
    ];
    people.push(pf.gen_person("くまさん", 42, "お山の中"));
    people.push(pf.gen_person("ぱんだま", 43, "ひみちゅ"));
    people.push(pf.gen_person("ぱんだみ", 43, "ひみちゅ"));

    for p in &people {
        p.print(false);  // print_person(&p);
    }
    //for p in &people {
    //    print_person(&p);
    //}
    //for p in &people {
    //    print_person(p);
    //}
    //for p in &people {
    //    print_person(&p);
    //}

    //struct Color(f32, f32, f32);
    //let yellow = Color(1.0, 1.0, 0.0);
    //let (r, g, b) = yellow;
    //println!("RGB = ({:.2}, {:.2}, {:.2})", r, g, b);

    let rect = Rectangle {
        left: 3.5,
        top: -2.5,
        width: 12.0,
        height: 4.5,
    };
    let mut tri = Polygon {
        vertexes: [(-12.0, 30.0), (4.0, -18.0), (22.0, 13.0)],
    };
    let tri2 = Polygon::<f32> {
        vertexes: [(-12.0, 30.0), (4.0, -18.0), (22.0, 13.0)],
    };
    let cir = Circle {
        center: (20.0, 0.0),
        radius: 6.0,
    };
    print_area(&rect, "rect");
    print_area(&tri, "tri");
    print_area(&tri2, "tri2");
    print_area(&cir, "cir");
    //println!("rect area is {}", rect.calc_area());
    //println!("tri area is {}", tri.calc_area());
    //println!("cir area is {}", cir.calc_area());


    let mut tri_ref = &mut tri;
    tri_ref.vertexes[1] = (0.0, 0.0);   // 参照のメンバーも "." でアクセス可能
    // 以下 2行は順序を逆にはできない (可変参照を操作中に参照元を借用することはできない)
    print_area(tri_ref, "tri_ref");     // 既に参照なので "&" は不要
    print_area(&tri, "tri");

    // このあとに tri_ref を使用しないのであれば、同一スコープ内で mutable な参照を
    // もう一つ借用することも可能
    let mut tri_ref2 = &mut tri;
    tri_ref2.vertexes[2] = (-22.0, 13.0);
    print_area(tri_ref2, "tri_ref2");
    print_area(&tri, "tri");
}

fn print_area<T: CalcArea>(fig: &T, name: &str) {
    println!("{} area is {}", name, fig.calc_area());
}

#[allow(dead_code)]
fn new_person(id: &mut i32, name: &str, age: i32, addr: &str) -> Person {
    let pa = Person {
        id: *id,
        name: name.to_string(),
        age: age,
        addr: addr.to_string(),
    };
    *id += 1;
    pa
}

#[allow(dead_code)]
fn print_person(pa: &Person) {
    println!("{}: {} ({}) in {}", pa.id, pa.name, pa.age, pa.addr);
}
