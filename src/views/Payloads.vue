
<template>
  <div class="payloads">
    <h1>Payloads</h1>
    <div class="container">
        <h3>Usages</h3>
        <gauge-bar
            :max-value="info.total"
            :current-value="info.used"
            :value-formatter="numberFormat"
            :show-total="true">
        </gauge-bar>
        <label class="hints">*There are some spaces allocated for the user interface code</label>
        <h3>Manage</h3>
        <table class="payloads-list">
          <payload-info v-for="p in payloads" :key="p.key"
            :payload="p"
            :total-kb="info.total"
            :value-formatter="numberFormat"
            :delete-handler="deletePayload">
          </payload-info>
        </table>
    </div>
  </div>
</template>

<script>
import { mapState } from 'vuex';
import GaugeBar from '@/components/GaugeBar';
import PayloadInfo from '@/components/PayloadInfo';

export default {
  name: 'payloads',
  components: {
    [GaugeBar.name]: GaugeBar,
    [PayloadInfo.name]: PayloadInfo,
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
    deletePayload(key) {
      console.log(key);
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

    .payloads-list {
      width: 100%;
    }
  }
}
</style>
