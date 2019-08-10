export function range(start, count) {
	return Array(count)
		.fill(0)
		.map((_, index) => { return index + start;});
}

export function arrayIntersection(array1, array2) {
	return array1.filter(val => array2.includes(val));
}

export function arrayDifference(array1, array2) {
	return array1.filter(val => !array2.includes(val));
}

export function mapKeys(object, transformFunction) {
	let keys = Object.keys(object);
	let newObject = {};

	keys.forEach(key => {
		let newKey = transformFunction(key);
		newObject[newKey] = object[key]
	});

	return newObject;
}

// https://stackoverflow.com/a/2117523
export function uuid4() {
	return 'xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx'.replace(/[xy]/g, (c) => {
		let r = Math.random() * 16 | 0, v = c === 'x' ? r : (r & 0x3 | 0x8);
		return v.toString(16);
	});
}
