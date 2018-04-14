
<template>
  <div class="payloads">
    <h1>Payloads</h1>
    <div class="container">
        <h3>Usages</h3>
        <gauge-bar
            :max-value="this.info.total"
            :current-value="this.info.used"
            :value-formatter="numberFormat">
        </gauge-bar>
        <label class="hints">*There are some spaces allocated for the user interface code</label>
        <h3>Manage</h3>
        <table>
            <tr v-for="p in payloads" :key="p.key">
                <td>{{ p.name }}</td>
            </tr>
        </table>
    </div>
  </div>
</template>

<script>
import { mapState } from 'vuex';
import GaugeBar from '@/components/GaugeBar';

export default {
  name: 'payloads',
  components: {
    [GaugeBar.name]: GaugeBar,
  },
  data: () => ({
    info: {},
  }),
  created() {
    this.$http.getFileSystem(result => {
      this.info = result;
    });
  },
  methods: {
    numberFormat(val) {
      return `${(val / 1024.0).toFixed(2)}Kb`;
    },
  },
  computed: {
    ...mapState(['payloads']),
  },
};
</script>

<style lang="less">
.payloads {
  .container {
    text-align: left;
  }
}
</style>
