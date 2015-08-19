
declare enum FacerEventType {
	NoType,
	MouseMove,
	MousePress,
	MouseRelease,
	MouseWheel,
	KeyPress,
	KeyRelease,
	InputText
}

declare enum FacerMouseButton {
	NOBUTTON,
	LEFT,
	RIGHT,
	MIDDLE
}

declare enum FacerButtonState {
	RELEASED, PRESSED }

declare class FacerEventMouseButtonState {
	left: FacerButtonState;
	right: FacerButtonState;
	middle: FacerButtonState;
}

declare class FacerEventModifierState {
	alt: boolean;
	shift: boolean;
	meta: boolean;
	control: boolean;
}

declare class FacerEvent {
	type: FacerEventType;
	primaryButton: FacerMouseButton;
	buttonState: FacerEventMouseButtonState;
	modifierState: FacerEventModifierState;
	input: string;
	timestamp: number;

	isMouseCursorEvent(): boolean;
	setCursorPosition(xpos: number, ypos: number): FacerEvent;
	setWheelDelta(xdel: number, ydel: number): FacerEvent;

	cursorPositionX: number;
	cursorPositionY: number;
	wheelDeltaX: number;
	wheelDeltaY: number;
}
