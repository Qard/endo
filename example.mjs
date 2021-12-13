import { sleep } from 'timer'

print("argv:", ...ARGV)

print('hello...')
await sleep(500)
print('...Stephen!')

await sleep(100)

print('hello...')
await sleep(1000)
print('...world!')
