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
