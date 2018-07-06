module Point = {
  type t = {
    x: int,
    y: int,
  };
};

module Delta = {
  type t = {
    x: int,
    y: int,
  };
};

module Dimensions = {
  type t = {
    width: int,
    height: int,
  };
};

module Rect = {
  type t = {
    top: int,
    bottom: int,
    left: int,
    right: int,
  };
};

module Margins = {
  type t = {
    top: int,
    bottom: int,
    left: int,
    right: int,
  };
};

module Borders = {
  type t = {
    top: int,
    bottom: int,
    left: int,
    right: int,
  };
};

module Paddings = {
  type t = {
    top: int,
    bottom: int,
    left: int,
    right: int,
  };
};

module Offset = {
  type t = {
    top: int,
    bottom: int,
    left: int,
    right: int,
  };
};

module Scroll = {
  type t = {
    initial: Point.t,
    current: Point.t,
    delta: Delta.t,
    max: Dimensions.t,
  };
};

module Direction = {
  type t =
    | Alpha
    | Omega;
};

module RelativityBag = {
  type t('a) = {
    page: 'a,
    viewport: 'a,
  };
};

module Geometry = {
  type t = {
    rect: RelativityBag.t(Rect.t),
    dimensions: Dimensions.t,
    margins: Margins.t,
    borders: Borders.t,
    paddings: Paddings.t,
  };
};

module Shift = {
  type t = option(Direction.t);
};

module Scrollable = {
  type t = {
    element: Dom.htmlElement,
    geometry: Geometry.t,
  };
};

module DraggableBag = {
  type t('draggableId, 'droppableId) = {
    id: 'draggableId,
    droppableId: 'droppableId,
    geometry: option(Geometry.t),
    shift: Shift.t,
    animating: bool,
    getGeometry: unit => Geometry.t,
  };

  type registrationPayload('draggableId, 'droppableId) = {
    id: 'draggableId,
    droppableId: 'droppableId,
    getGeometry: unit => Geometry.t,
  };
};

module DroppableBag = {
  type t('draggableId, 'droppableId) = {
    id: 'droppableId,
    geometry: option(Geometry.t),
    scrollable: option(Scrollable.t),
    accept: option('draggableId => bool),
    getGeometryAndScrollable: unit => (Geometry.t, option(Scrollable.t)),
  };

  type registrationPayload('draggableId, 'droppableId) = {
    id: 'droppableId,
    accept: option('draggableId => bool),
    getGeometryAndScrollable: unit => (Geometry.t, option(Scrollable.t)),
  };
};

module Ghost = {
  type t('draggableId, 'droppableId) = {
    element: Dom.htmlElement,
    draggableId: 'draggableId,
    originalDroppable: 'droppableId,
    targetDroppable: option('droppableId),
    targetingOriginalDroppable: bool,
    direction: option(Direction.t),
    dimensions: Dimensions.t,
    margins: Margins.t,
    borders: Borders.t,
    delta: Delta.t,
    departurePoint: RelativityBag.t(Point.t),
    currentPoint: RelativityBag.t(Point.t),
    departureRect: RelativityBag.t(Rect.t),
    currentRect: RelativityBag.t(Rect.t),
  };
};

module Subscriptions = {
  type t = {
    install: unit => unit,
    drop: unit => unit,
  };
};

module Status = {
  type t('draggableId, 'droppableId) =
    | StandBy
    | Dragging(Ghost.t('draggableId, 'droppableId), Subscriptions.t)
    | Dropping(Ghost.t('draggableId, 'droppableId));
};

module Context = {
  type t('draggableId, 'droppableId) = {
    status: Status.t('draggableId, 'droppableId),
    target: option('droppableId),
    scroll: option(Scroll.t),
    registerDraggable:
      DraggableBag.registrationPayload('draggableId, 'droppableId) => unit,
    registerDroppable:
      DroppableBag.registrationPayload('draggableId, 'droppableId) => unit,
    disposeDraggable: 'draggableId => unit,
    disposeDroppable: 'droppableId => unit,
    getDraggableShift: 'draggableId => Shift.t,
    initDrag:
      (
        ~draggableId: 'draggableId,
        ~droppableId: 'droppableId,
        ~start: RelativityBag.t(Point.t),
        ~current: RelativityBag.t(Point.t),
        ~element: Dom.htmlElement,
        ~subscriptions: Subscriptions.t
      ) =>
      unit,
    updateGhostPosition: RelativityBag.t(Point.t) => unit,
    drop: unit => unit,
    cancelDrag: unit => unit,
  };
};

module Payload = {
  type t('draggableId, 'droppableId) = {
    context: Context.t('draggableId, 'droppableId),
  };
};

module DropResult = {
  type droppables('droppableId) = {
    prev: 'droppableId,
    next: 'droppableId,
  };

  type t('draggableId, 'droppableId) =
    | SameTarget('draggableId, 'droppableId, array('draggableId))
    | NewTarget('draggableId, droppables('droppableId), array('draggableId))
    | NoChanges;

  type trait =
    | Ghost
    | Item(Shift.t);

  type draggableIntermediateResult('draggableId) = {
    id: 'draggableId,
    trait,
    rect: RelativityBag.t(Rect.t),
    margins: Margins.t,
  };
};
