f32
{
  create(some: f64): f32
  {
    :::convf32(some)
  }

  +(self: f32, other: f32): f32
  {
    :::add(self, other)
  }

  -(self: f32, other: f32): f32
  {
    :::sub(self, other)
  }

  *(self: f32, other: f32): f32
  {
    :::mul(self, other)
  }

  /(self: f32, other: f32): f32
  {
    :::div(self, other)
  }

  %(self: f32, other: f32): f32
  {
    :::mod(self, other)
  }

  **(self: f32, other: f32): f32
  {
    :::pow(self, other)
  }

  ==(self: f32, other: f32): bool
  {
    :::eq(self, other)
  }

  !=(self: f32, other: f32): bool
  {
    :::ne(self, other)
  }

  <(self: f32, other: f32): bool
  {
    :::lt(self, other)
  }

  <=(self: f32, other: f32): bool
  {
    :::le(self, other)
  }

  >(self: f32, other: f32): bool
  {
    :::gt(self, other)
  }

  >=(self: f32, other: f32): bool
  {
    :::ge(self, other)
  }

  min(self: f32, other: f32): f32
  {
    :::min(self, other)
  }

  max(self: f32, other: f32): f32
  {
    :::max(self, other)
  }

  logbase(self: f32, other: f32): f32
  {
    :::logbase(self, other)
  }

  atan2(self: f32, other: f32): f32
  {
    :::atan2(self, other)
  }

  -(self: f32): f32
  {
    :::neg(self)
  }

  abs(self: f32): f32
  {
    :::abs(self)
  }

  ceil(self: f32): f32
  {
    :::ceil(self)
  }

  floor(self: f32): f32
  {
    :::floor(self)
  }

  exp(self: f32): f32
  {
    :::exp(self)
  }

  log(self: f32): f32
  {
    :::log(self)
  }

  sqrt(self: f32): f32
  {
    :::sqrt(self)
  }

  cbrt(self: f32): f32
  {
    :::cbrt(self)
  }

  isinf(self: f32): bool
  {
    :::isinf(self)
  }

  isnan(self: f32): bool
  {
    :::isnan(self)
  }

  sin(self: f32): f32
  {
    :::sin(self)
  }

  cos(self: f32): f32
  {
    :::cos(self)
  }

  tan(self: f32): f32
  {
    :::tan(self)
  }

  asin(self: f32): f32
  {
    :::asin(self)
  }

  acos(self: f32): f32
  {
    :::acos(self)
  }

  atan(self: f32): f32
  {
    :::atan(self)
  }

  sinh(self: f32): f32
  {
    :::sinh(self)
  }

  cosh(self: f32): f32
  {
    :::cosh(self)
  }

  tanh(self: f32): f32
  {
    :::tanh(self)
  }

  asinh(self: f32): f32
  {
    :::asinh(self)
  }

  acosh(self: f32): f32
  {
    :::acosh(self)
  }

  atanh(self: f32): f32
  {
    :::atanh(self)
  }

  e(): f32
  {
    f32 :::e()
  }

  pi(): f32
  {
    f32 :::pi()
  }

  inf(): f32
  {
    f32 :::inf()
  }

  nan(): f32
  {
    f32 :::nan()
  }

  bool(self: f32): bool
  {
    self != 0.f32
  }

  i8(self: f32): i8
  {
    :::convi8(self)
  }

  i16(self: f32): i16
  {
    :::convi16(self)
  }

  i32(self: f32): i32
  {
    :::convi32(self)
  }

  i64(self: f32): i64
  {
    :::convi64(self)
  }

  u8(self: f32): u8
  {
    :::convu8(self)
  }

  u16(self: f32): u16
  {
    :::convu16(self)
  }

  u32(self: f32): u32
  {
    :::convu32(self)
  }

  u64(self: f32): u64
  {
    :::convu64(self)
  }

  ilong(self: f32): ilong
  {
    :::convilong(self)
  }

  ulong(self: f32): ulong
  {
    :::convulong(self)
  }

  isize(self: f32): isize
  {
    :::convisize(self)
  }

  usize(self: f32): usize
  {
    :::convusize(self)
  }

  f32(self: f32): f32
  {
    self
  }

  f64(self: f32): f64
  {
    :::convf64(self)
  }
}
