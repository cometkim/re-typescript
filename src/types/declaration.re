type t = {name: string};

module DeclarationMap =
  Core.Map.Make({
    type value = t;
  });
