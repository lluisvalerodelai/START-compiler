import { requestTokens } from './tokenRequester';

async function main() {
	try {
		const tokens = await requestTokens("This/file/is/going/to/be/parsed");
		console.log('Tokens:', tokens);
	} catch (error) {
		console.error('Error:', error);
	}
}

main();